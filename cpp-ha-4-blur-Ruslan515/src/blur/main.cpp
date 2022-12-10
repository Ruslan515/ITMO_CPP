#include <iostream>
#include <string>
#include <fstream>
#include <limits.h>
#include <unistd.h>
#include <cmath>
#include <map>

#include "main.h"

const int R = 0;
const int IN_FILE = 1;
const int OUT_FILE = 2;

void create_kernel(double * kernel, int kernel_size) {
      double sigma = 0.3 * ((kernel_size - 1) * 0.5 - 1) + 0.8;
      double mean = kernel_size / 2;
      double sum = 0;
      // create kernel values
      for (int x = 0; x < kernel_size; ++x) {
            for (int y = 0; y < kernel_size; ++y) {
                  kernel[x * kernel_size + y] = (
                        exp(
                              -0.5 * (
                                    pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma,2.0)
                                    )
                              )
                         / (2 * M_PI * sigma * sigma));
                  sum += kernel[x * kernel_size + y];
            }
      }
      // kernel normalize
      for (int x = 0; x < kernel_size; ++x) {
            for (int y = 0; y < kernel_size; ++y) {
                  kernel[x * kernel_size + y] /= sum;
            }            
      }

      return;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
      if (argc != 7) {
            std::cerr << "no args\n";             
            return 1;
      }      

      std::string file_name_in;
      std::string file_name_out;
      int r = 1;
      std::map <std::string, int> mapping;
      mapping["-r"] = R;
      mapping["-i"] = IN_FILE;
      mapping["-o"] = OUT_FILE;
      std::string radius;
      for (int idx = 1; idx < argc; idx += 2) {
            int command = mapping[argv[idx]];
            switch (command)
            {
            case R:
                  radius = argv[idx + 1];
                  r = std::stoi(radius);
                  if (r % 2 == 0) {
                        std::cerr << "radius not odd\n";
                        return 2;
                  }
                  break;
            case IN_FILE:
                  file_name_in = argv[idx + 1];

                  break;                        
            default:
                  file_name_out = argv[idx + 1];
                  break;
            }
      }
      
      // std::string dir = "../build/test/";
      // file_name_in = dir + file_name_in;

      std::ifstream file(file_name_in, std::ios::binary);
      if (file.bad()) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                  printf("dir %s\n", cwd);
            }
            // std::cout << "=================\n" << "\tError\t" <<  "=================\n";
            std::cerr << "file not exists\n";
            return 3;
      }

      BMPHeader file_header;
      file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));

      BITMAPINFOHEADER info_header;
      file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
      // std::cout << info_header.biWidth << " " << info_header.biHeight << " " << info_header.biBitCount << "\n";
      
      int h, w;
      h = info_header.biHeight;
      w = info_header.biWidth;
      
      // read to the one data
      int size_data = h * w * info_header.biBitCount / 8u;
      std::uint8_t *data;
      data = (std::uint8_t *) malloc (size_data * sizeof(std::uint8_t));
      // std::cout << sizeof(data) << "\n";
      // std::cout << size_data << "\n";
      file.read(reinterpret_cast<char*>(data), size_data);      
      file.close();      


      std::ifstream file_for_out(file_name_in, std::ios::binary);
      file_for_out.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
      file_for_out.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
      std::uint8_t * out_data;
      out_data = (std::uint8_t *) malloc (size_data * sizeof(std::uint8_t));
      file_for_out.read(reinterpret_cast<char*>(out_data), size_data);      
      file_for_out.close();      


      // for (auto i = 0; i < size_data; i += 3) {
      //       std::uint8_t temp = data[i];
      //       data[i] = data[i + 2];
      //       data[i + 2] = temp;
      // }

      // test
      // std::ofstream test("../../test/test.bmp", std::ios::binary);
      // test.write((char*)(&file_header), sizeof(file_header));
      // test.write((char*)(&info_header), sizeof(info_header));
      // test.write((char*)(data), size_data);

      double *kernel;
      
      // int kernel_size = (2 * r) + 1;
      int kernel_size = r;

      kernel = (double *) malloc (kernel_size * kernel_size * sizeof(double));
      create_kernel(&kernel[0], kernel_size);
      
      // std::cout << "kernel\n";
      // int xx = r / 2;
      // xx *= kernel_size;
      // xx += r / 2;
      // std::cout << xx << "\n";
      // std::cout << kernel[xx] << "\n";
      
      // for (auto x = 0; x < kernel_size; ++x) {
      //       for (auto y = 0; y < kernel_size; ++y) {
      //             std::cout << kernel[x * kernel_size + y] << " ";
      //       }
      //       std::cout << "\n";
      // }


      int padding = r / 2;
      // std::cout << padding << "\n";

      // for (auto chanel = 0; chanel < 3; chanel++) {
      //       for (auto x = padding; x < (h - padding); x++) {
      //             for (auto y = padding; y < (w - padding); y++) {
      //                   double ans = 0;
      //                   if (padding == 0) {
      //                         ans = data[3 * (x * w + y) + chanel];
      //                   }
      //                   else {
      //                         for (auto x_kernel = -padding; x_kernel < padding; x_kernel++) {
      //                               for (auto y_kernel = -padding; y_kernel < padding; y_kernel++) {
      //                                     int x_new = x + x_kernel;
      //                                     int y_new = y + y_kernel;
      //                                     int d = x_new * w + y_new;
      //                                     int coord = 3 * d + chanel;

      //                                     double temp = data[coord];

      //                                     int x_new_kernel = x_kernel + padding;
      //                                     x_new_kernel *= r;
      //                                     int y_new_kernel = y_kernel + padding;
      //                                     double data_kernel = kernel[x_new_kernel + y_new_kernel];
      //                                     // temp *= kernel[(x_kernel + padding) * r + (y_kernel + padding)];
      //                                     temp *= data_kernel;
      //                                     ans += temp;
      //                               }
      //                         }

      //                   }

      //                   out_data[3 * (x * w + y) + chanel] = ans;
      //             }
      //       }

      // }

      for (auto chanel = 0; chanel < 3; chanel++) {
            for (auto x = padding; x < (h - padding); x++) {
                  for (auto y = padding; y < (w - padding); y++) {
                        double ans = 0;
                        if (padding == 0) {
                              ans = data[3 * (x * w + y) + chanel];
                        }
                        else {
                              for (auto x_kernel = -padding; x_kernel < padding; x_kernel++) {
                                    for (auto y_kernel = -padding; y_kernel < padding; y_kernel++) {
                                          double kernel_val = kernel[(x_kernel + padding) * r + y_kernel + padding];
                                          double temp = data[3 * ((x + x_kernel) * w + (y + y_kernel)) + chanel];
                                          temp *= kernel_val;
                                          ans += temp;
                                    }
                              }

                        }
                        // int xx = x * w;
                        // int xxx = 3 * (x * w + y);

                        // out_data[3 * (x * w + y) + chanel] = ans;
                        out_data[3 * (x * w + y) + chanel] = ans;
                  }
            }

      }

      // std::string dir_out = "../test/";
      // file_name_out = dir_out + file_name_out;
      std::ofstream file_out(file_name_out, std::ios::binary);
      file_out.write((char*)(&file_header), sizeof(file_header));
      file_out.write((char*)(&info_header), sizeof(info_header));
      file_out.write((char*)(out_data), size_data);
      
      file_out.close();

      // FILE *f = fopen(file_name_out, "wb");
      // fwrite(file_header, 1, 14, f);
      // fwrite(info_header, 1, 40, f);


      free(kernel);
      free(data);
      free(out_data);
      return 0;
}

// /media/ruslan515/DATA_LINUX/ITMO/I_semestr/C++/hw_04_copy/build/src/blur -i helena_500x500.bmp -o helena_500x500_out_x5.bmp -r 5
// compare -fuzz 5% -metric AE helena_500x500_filtered_5.bmp helena_500x500_out_x5.bmp helena_500x500_out_x5.bmp.diff.png
