// conv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <time.h>
#include <windows.h>
#include "conv4.h"
/*conv4.h内容：
#define INP_IMAGE_SIZE 15                    //输入大小
#define INP_IMAGE_CHANNEL 384         //输入通道数
#define OUT_IMAGE_SIZE 13                  //输出图像大小
#define FILTER_BATCH 384                     //卷积核输入通道数
#define FILTER_CHANNEL 192	              //卷积核个数
#define FILTER_SIZE 3                            //卷积核大小
#define STRIDE 1                                   //步长
#define STRIDE_NEW 3                          //新步长
#define GROUP 2
*/

typedef float DataType;//DataType表示浮点型数据

using namespace std;

void conv4(/*DataType *inp_img, DataType *out_img, DataType *filter, DataType *bias*/
	DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],//15 × 15 × 384
	DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH],//13 × 13 × 384
	DataType filter[FILTER_BATCH * INP_IMAGE_CHANNEL * FILTER_SIZE * FILTER_SIZE],//384 × 384 × 3 × 3
	DataType bias[FILTER_BATCH]);//384


int main()
{

	ifstream inp_file("inp_conv4.txt");                                                 //ifstream输入文件流input file stream用于从文件读数据(从文件读入)
																			   //从当前目录下的inp_conv4.txt中读取内容
	DataType *inp_image;				       //浮点型指针变量inp_image

	inp_image = (DataType *)malloc(INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL * sizeof(DataType));//malloc动态内存分配，函数原型为void *malloc(unsigned int size);其作用是在内存的动态存储区中
														  //分配一个长度为size的连续空间，此函数的返回值是分配区域的起始地址，或者说，此函数是一个指
											  //针型函数，返回的指针指向该分配域的开头位置。
											  //此处动态分布大小为15*15*384字节的空间，该空间的内容为输入的信息
											  //回忆：int *a; a = &p;//a指向地址值为p的空间
											  //int *a; *a = p;//p的值赋给a所指向的空间
	if (inp_file.is_open())						                                           //.open()打开；.is_open()打开成功返回1，失败返回0。
	{
		cout << "can open the text file" << endl;			                                          //屏幕上输出can open the text file


		for (int i = 0; i < INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL; i++)               //遍历输入的所有信息
		{
			inp_file >> inp_image[i];
			//cout<< "inp_image[" << i << "] = " << inp_image[i] << endl;
		}
		inp_file.close();							         //关闭 
	}
	else
		printf("fail");


	ifstream filter_file("conv4Weights.txt");//从当前目录下的conv4Weights.txt中读取内容
	DataType *filter;//浮点型指针变量filter
	filter = (DataType *)malloc(FILTER_BATCH*INP_IMAGE_CHANNEL*FILTER_SIZE*FILTER_SIZE * sizeof(DataType));
	if (filter_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i = 0; i < FILTER_BATCH*INP_IMAGE_CHANNEL*FILTER_SIZE*FILTER_SIZE; i++)
		{
			filter_file >> filter[i];
			//cout<< "inp_image[" << i << "] = " << inp_image[i];
		}
		filter_file.close();
	}

	ifstream bias_file("conv4Bias.txt");//打开偏置配置文件
	DataType *bias;
	bias = (DataType *)malloc(FILTER_BATCH * sizeof(DataType));
	if (bias_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i = 0; i < FILTER_BATCH; i++)
		{
			bias_file >> bias[i];
			//cout<< "inp_image[" << i << "] = " << inp_image[i];
		}
		bias_file.close();
	}

	DataType *out_image = (DataType *)malloc(OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH * sizeof(DataType));

	cout << "Start calling the conv4 function" << endl;//开始调用conv1 HW函数

	conv4(inp_image, out_image, filter, bias);

//	ofstream out_file("out_img.txt");
//	if (out_file.is_open())
//	{
//		for (int i = 0; i < OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH; i++)
//		{
//			out_file << out_image[i] << "\n";
//		}
//		cout << "can write the text file" << endl;
//		out_file.close();
//	}
//	else
//		printf("fail");

	return 0;
}

void conv4(/*DataType *inp_img, DataType *out_img, DataType *filter, DataType *bias*/
	DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],//15 × 15 × 384
	DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH],//13 × 13 × 384
	DataType filter[FILTER_BATCH * INP_IMAGE_CHANNEL * FILTER_SIZE * FILTER_SIZE],//384 × 384 × 3 × 3
	DataType bias[FILTER_BATCH])//384
{
	LARGE_INTEGER  large_interger;
	double dff;
	__int64  c1, c2;
	QueryPerformanceFrequency(&large_interger);
	dff = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	c1 = large_interger.QuadPart;

	cout << "Start the conv4 function" << endl;
	DataType inp_img_2D[INP_IMAGE_SIZE][INP_IMAGE_SIZE];//15 15
	DataType filter_2D[FILTER_SIZE][FILTER_SIZE];//3 3
//	DataType filter_local[INP_IMAGE_CHANNEL * FILTER_SIZE*FILTER_SIZE];
	DataType window_2D[FILTER_SIZE][FILTER_SIZE];//3 3
	DataType conv_out[OUT_IMAGE_SIZE*OUT_IMAGE_SIZE];//13 × 13
	DataType sh[FILTER_SIZE][FILTER_SIZE];//3 3 */
	DataType lm;

	for (int batch = 0; batch < FILTER_BATCH; batch++) 
	{
		//batch n. 一批，一炉；将384张输出特征图分批次，即384次处理。整个代码的目的是每次生成一幅输出特征图，结束后在这个for循环外向out_img填充所有结果。 

//		for (int h = 0; h < FILTER_SIZE * FILTER_SIZE * FILTER_BATCH; h++) 
//		{
//		filter_local[h] = filter[batch*FILTER_BATCH*FILTER_SIZE*FILTER_SIZE + h];
//		}//取384×3×3×384个卷积核中的384×3×3，batch每变一次，则会换一组。
		for (int i = 0; i < OUT_IMAGE_SIZE*OUT_IMAGE_SIZE; i++)//13 × 13
			conv_out[i] = 0;//将一幅输出特征图(临时可变)全部置零
		for (int chan = 0; chan < INP_IMAGE_CHANNEL; chan++) 
		{
			//384对应输入通道的计算，每次变化意味着一张图处理完成
			for (int ii = 0; ii < INP_IMAGE_SIZE; ii++) 
			{//15
				for (int jj = 0; jj < INP_IMAGE_SIZE; jj++) 
				{//15 inp_img大小为15×15×384，下一步人为生成一个二维数组，仿真成15×15×384的输入图片(inp_img_2D[14][14]为第一幅图的最后一个点 )
					inp_img_2D[ii][jj] = inp_img[chan*INP_IMAGE_SIZE*INP_IMAGE_SIZE + ii * INP_IMAGE_SIZE + jj];//chan×15×15+ii×15+jj，jj为一个方向上取点(如按行)最大取到14，表明，ii×15+jj当一行取完后ii
				}
			}//end ii 每一次循环结束意味着已经将大小为一幅图(输入)的一维信息存储为二维信息
			for (int ii = 0; ii < FILTER_SIZE; ii++) 
			{//3
				for (int jj = 0; jj < FILTER_SIZE; jj++) 
				{//3
					filter_2D[ii][jj] = filter[batch*FILTER_SIZE*FILTER_SIZE + chan * FILTER_SIZE*FILTER_SIZE + ii * FILTER_SIZE + jj];//batch×3×3+chan×3×3+ii×3+jj
				}
			}//end ii 每一次循环结束意味着已经将大小为一幅图(卷积核)的一维信息存储为二维信息
			for (int row = FILTER_SIZE - 1; row < INP_IMAGE_SIZE; row += STRIDE) 
			{
				//行 = 2；行 < 15；行 = 行 + 步长
				for (int col = FILTER_SIZE - 1; col < INP_IMAGE_SIZE; col += STRIDE) 
				{
					// 2   <15  ++
					for (int i = 0; i < FILTER_SIZE; i++) 
					{//i < 3
						for (int j = 0; j < FILTER_SIZE; j++) 
						{//j < 3
							window_2D[i][j] = inp_img_2D[row - FILTER_SIZE + 1 + i][col - FILTER_SIZE + 1 + j];//[row-2+i][col-2+j]
						}
					}//end i window_2D实现在一张输入特征图(二维信息)截取3×3的区域
					for (int ii = 0; ii < FILTER_SIZE; ii++) 
					{//<3
						for (int jj = 0; jj < FILTER_SIZE; jj++) 
						{//<3
							sh[ii][jj] = filter_2D[ii][jj] * window_2D[ii][jj];//与row，col结合上面只要对一个3×3的方阵完成取点，此时就会计算一次并存储到sh二维数组
						}
					}//end ii sh存储3×3大小的部分输出特征值(即单张输出特征图的单个神经元的感受野和卷积核完成运算)
					lm = 0;//用于累计，清一次0意味着开始计算当前神经元(输出特征图)
					for (int i = 0; i < FILTER_SIZE; i++) 
					{
						for (int j = 0; j < FILTER_SIZE; j++) 
						{
							lm += sh[i][j];//存储完后累加完成卷积操作在输出图片上会得到编号为0的第一个卷积结果
						}
					}//end i 完成输出特征图上单个神经元的求解
					conv_out[((row - FILTER_SIZE + 1) / STRIDE)*OUT_IMAGE_SIZE + (col - FILTER_SIZE + 1) / STRIDE] += lm;//(row-2)×13+(col-2)，chan的变化和+=实现了对同一位置不同特征图的累加					
				}//end col 完成对输出特征图一行神经元的计算，row接下来+1意味着换一次行				
			}//end row 完成一张输入特征图的计算即得到一张13×13的输出特征图			
		}//end chan 在不考虑卷积核输出通道的情况下完成384组计算(即单输出通道下处理完384个输入特征图)
		for (int i = 0; i < OUT_IMAGE_SIZE; i++) 
		{//13
			for (int j = 0; j < OUT_IMAGE_SIZE; j++) 
			{//13
				DataType out = conv_out[i*OUT_IMAGE_SIZE + j] + bias[batch];//(i×13+j)+对应偏置 给一幅图中的每个输出添加偏置
				if (out > 0)//激活函数
					out_img[batch*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i * OUT_IMAGE_SIZE + j] = out;//batch×13×13+i×13+j
				else
					out_img[batch*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i * OUT_IMAGE_SIZE + j] = 0;
			}
		}
	}//end of batch 完成384×13×13的输出*/
	QueryPerformanceCounter(&large_interger);
	c2 = large_interger.QuadPart;

	printf("time=%lfs\n", (c2 - c1) / dff);
}//end of conv4



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件