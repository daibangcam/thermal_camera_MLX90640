#/*MIT License
#Copyright (c) 2018 imliubo
#Github  https://github.com/imliubo
#Website https://www.makingfun.xyz
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.
#*/
CC = g++
CXXFLAGS = -O -g `sdl2-config --cflags --libs` 
LFLAGS=`sdl2-config --libs` -lSDL2_ttf
OBJS = main.o libMLX90640_API.a
INCLUDE += -I ./Include -I ./Include/mlx90640-driver
TARGET = main

vpath %.cpp ./Source ./Source/mlx90640-driver
vpath %.c ./Source ./Source/mlx90640-driver

all:main.o MLX90640_API.o libMLX90640_API.a
	$(CC) $(CXXFLAGS) $(LFLAGS) $(INCLUDE) -o $(TARGET) $(OBJS)
	rm -f *.o *a
	
main.o:main.cpp 
	$(CC) $(CXXFLAGS) $(LFLAGS) $(INCLUDE) -c $^

libMLX90640_API.a:MLX90640_API.o MLX90640_LINUX_I2C_Driver.o
	ar rcs $@ $^
	ranlib $@
	
MLX90640_API.o:MLX90640_API.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

MLX90640_LINUX_I2C_Driver.o:MLX90640_LINUX_I2C_Driver.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

.PHONY:clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f $(TARGET)

