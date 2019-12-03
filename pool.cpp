#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <windows.h>
#include "pool5.h"

/*
#define INP_IMG_CHAN 384
#define INP_IMG_SIZE 13
#define OUT_IMG_SIZE 6
#define FILTER_SIZE 3
#define STRIDE 2         步长为2
*/

typedef float DataType;
using namespace std;

void pool5(DataType inp_img[INP_IMG_CHAN*INP_IMG_SIZE*INP_IMG_SIZE], DataType out_img[INP_IMG_CHAN*OUT_IMG_SIZE*OUT_IMG_SIZE]);//inp_img[13][13] [384](大小为13×13，通道数为384，即有384个图像)；
														//out_img[6][6][384](大小为6×6) ((13+2×0-3)/2+1) = 6

//main function used to test the functionality of the kernel.
int main()
{

	ifstream inp_file("out_img.txt");                             //ifstream输入文件流input file stream用于从文件读数据(从文件读入)
	DataType *inp_image;

	inp_image = (DataType *)malloc(INP_IMG_SIZE * INP_IMG_SIZE * INP_IMG_CHAN * sizeof(DataType));//动态分配了大小为13×13×384(数据个数)×float(字节空间)
	if (inp_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i = 0; i < INP_IMG_SIZE * INP_IMG_SIZE * INP_IMG_CHAN; i++)//遍历13×13×384个数据
		{
			inp_file >> inp_image[i];
		}
		inp_file.close();
	}

	DataType *out_image = (DataType *)malloc(OUT_IMG_SIZE * OUT_IMG_SIZE * INP_IMG_CHAN * sizeof(DataType));//6×6×384
	cout << "Start calling the conv1 HW function" << endl;

	pool5(inp_image, out_image);//将imp_image地址和out_image的地址作为形参，传递给pool5函数

//	ofstream out_file("out_pool.txt");
//	if (out_file.is_open())
//	{
//		for (int i = 0; i < OUT_IMG_SIZE * OUT_IMG_SIZE * INP_IMG_CHAN; i++)
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


void pool5(DataType inp_img[INP_IMG_CHAN*INP_IMG_SIZE*INP_IMG_SIZE], DataType out_img[INP_IMG_CHAN*OUT_IMG_SIZE*OUT_IMG_SIZE])//pool5功能函数
{
	LARGE_INTEGER  large_interger;
	double dff;
	__int64  c1, c2;
	QueryPerformanceFrequency(&large_interger);
	dff = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	c1 = large_interger.QuadPart;

	DataType window_2D[FILTER_SIZE][FILTER_SIZE];
	DataType inp_img_2D[INP_IMG_SIZE][INP_IMG_SIZE];//13 13 
	for (int chan = 0; chan < INP_IMG_CHAN; chan++)	//384对应输入通道的计算，每次变化意味着一张图处理完成
	{
		for (int ii = 0; ii < INP_IMG_SIZE; ii++)
		{//13
			for (int jj = 0; jj < INP_IMG_SIZE; jj++)
			{//13 inp_img大小为13×13×384，下一步人为生成一个二维数组，仿真成13×13×384的输入图片(inp_img_2D[13][13]为第一幅图的最后一个点 )
				inp_img_2D[ii][jj] = inp_img[chan*INP_IMG_SIZE*INP_IMG_SIZE + ii * INP_IMG_SIZE + jj];//chan×13×13+ii×13+jj，jj为一个方向上取点(如按行)最大取到12，表明，ii×13+jj当一行取完后ii
			}
		}//end ii 每一次循环结束意味着已经将大小为一幅图(输入)的一维信息存储为二维信息
		for (int row = FILTER_SIZE - 1; row < INP_IMG_SIZE; row += STRIDE)
		{//行 = 2；行 < 13；行 = 行 + 步长
			for (int col = FILTER_SIZE - 1; col < INP_IMG_SIZE; col += STRIDE)
			{// 2   <13  ++
				for (int i = 0; i < FILTER_SIZE; i++)
				{//i < 3
					for (int j = 0; j < FILTER_SIZE; j++)
					{//j < 3
						window_2D[i][j] = inp_img_2D[row - FILTER_SIZE + 1 + i][col - FILTER_SIZE + 1 + j];//[row-2+i][col-2+j]
					}
				}//end i window_2D实现在一张输入特征图(二维信息)截取3×3的区域
				DataType tmp = 0;
				for (int i = 0; i < FILTER_SIZE; i++)//i < 3,即i = 0,1,2
				{
					for (int j = 0; j < FILTER_SIZE; j++)//j < 3,即j = 0,1,2
					{
						if (window_2D[i][j] > tmp)
							tmp = window_2D[i][j];
						else
							tmp = tmp;
					}
				}
				out_img[chan*OUT_IMG_SIZE*OUT_IMG_SIZE + ((row - FILTER_SIZE + 1) / STRIDE)*OUT_IMG_SIZE + (col - FILTER_SIZE + 1) / STRIDE] = tmp;//chan×6×6+[(row - 2)/2]×6+(col - 2)/2
			}//end col 完成对输出特征图一行神经元的计算，row接下来+1意味着换一次行
		}//end row 完成一张输入特征图的计算即得到一张13×13的输出特征图
	}//end chan 在不考虑卷积核输出通道的情况下完成384组计算(即单输出通道下处理完384个输入特征图)	
	QueryPerformanceCounter(&large_interger);
	c2 = large_interger.QuadPart;

	printf("time=%lfs\n", (c2 - c1) / dff);
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
