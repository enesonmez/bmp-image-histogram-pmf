#include <iostream>
#include <vector>
#include <cmath>
#include "bmp.cpp"

using namespace std;


int* Histogram(vector<uint8_t> data, int width, int height, int binlevel)
{
    int ptr = 0;
    int size = width * height;
    int* histData = new int[(int)(256/binlevel)] { 0 };

    while (ptr < size)
    {
        int h = (int)data[ptr];
        histData[h] +=1;
        ptr++;
    }
    return histData;
}


double Mean(const int* hist, int16_t size)
{
    int N = 0;
    int sum = 0;

    for ( int i = 0; i < size; i++ )
    {
        N  += hist[i];
        sum += hist[i] * i;
    }
    if ( N == 0 )
        return -1.0;
    else
        return sum / N;
}


double Variance(const int* hist, int16_t size)
{
    int N = 0;
    int sum = 0;
    double mean = Mean(hist, size);

    for ( int i = 0; i < size; i++ )
    {
        N  += hist[i];
        sum += (i-mean)*(i-mean) * hist[i];
    }
    if ( N == 0 )
        return -1.0;
    else
        return (sum / (N-1));
}


double Std(const int* hist, int16_t size)
{
    double variance = Variance(hist, size);
    if ( variance == -1.0 )
        return -1.0;
    else
        return sqrt(variance);
}


int Max(vector<uint8_t> data)
{
    int high = (int)data[0];
    for (int i = 1; i < data.size(); i++)
    {
        if (high < (int)data[i])
        {
            high = (int)data[i];
        }
    }
    return high;
}


int Min(vector<uint8_t> data)
{
    int low = (int)data[0];
    for (int i = 1; i < data.size(); i++)
    {
        if (low > (int)data[i])
        {
            low = (int)data[i];
        }
    }
    return low;
}


int Max_Element(int* hist, int size) {
	int high = hist[0];
	for (int i = 1; i < size; i++)
		if (high < hist[i]) high = hist[i];

	return high;
}


int main()
{
    BMP bmp("lena.bmp");
    int8_t BIN_LEVEL = 1;
    int32_t MAX_SQUARE = 50;
    int16_t HIST_SIZE = 256 / BIN_LEVEL;
    int32_t NUM_SAMPLES = bmp.bmp_info_header.width*bmp.bmp_info_header.height;



    int* hist = Histogram(bmp.data,bmp.bmp_info_header.width,bmp.bmp_info_header.height,BIN_LEVEL);
    int32_t square_count = Max_Element(hist,HIST_SIZE) / MAX_SQUARE;

    cout << "# Num Samples : " << NUM_SAMPLES << endl;
    cout << "# Min : " << Min(bmp.data) << endl;
    cout << "# Max : " << Max(bmp.data) << endl;
    cout << "# Mean : " << Mean(hist, HIST_SIZE) << endl;
    cout << "# Variance : " << Variance(hist, HIST_SIZE) << endl;
    cout << "# Standard Deviation : " << Std(hist, HIST_SIZE) << endl;
    cout << "# each " << (char)254u <<" represents a count of " << square_count << endl;
    cout << "# ------------------------------------------------" << endl;

//    int8_t bin_width = (Max(hist,HIST_SIZE)-Min(hist,HIST_SIZE))/BIN_LEVEL;



    return 0;
}
