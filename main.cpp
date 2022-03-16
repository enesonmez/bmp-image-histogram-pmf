#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "bmp.cpp"

using namespace std;

#define BINFORMAT "[%12.6f, %12.6f) |%9u| "


int* Histogram(vector<uint8_t> data, int width, int height, int binlevel, int min_value, int max_value)
{
    int ptr = 0;
    int size = width * height;
    int* histData = new int[binlevel] { 0 };

    while (ptr < size)
    {
        int binselect = ((int)data[ptr]-min_value)*binlevel/(max_value-min_value);
        /*int h = (int)data[ptr];
        histData[h] +=1;*/
        if (binselect < binlevel)
            histData[binselect]++;
        else
            histData[binlevel-1]++;
        ptr++;
    }
    return histData;
}


double Mean(const int* hist, int16_t size, int min_value, int max_value, int bin_width)
{
    int N = 0;
    int sum = 0;
    int count = 0;

    for ( int i = min_value; i < max_value; i+=bin_width )
    {

        N  += hist[count];
        sum += hist[count] * i;
        count++;
    }
    if ( N == 0 )
        return -1.0;
    else
        return sum / (double)N;
}


double Variance(const int* hist, int16_t size, int min_value, int max_value, int bin_width)
{
    int N = 0;
    double sum = 0;
    double mean = Mean(hist, size, min_value, max_value, bin_width);
    int count = 0;

    for ( int i = min_value; i < max_value; i+=bin_width )
    {
        N  += hist[count];
        sum += (i-mean)*(i-mean) * hist[count];
        count++;
    }
    if ( N == 0 )
        return -1.0;
    else
        return (sum / (N-1));
}


double Std(const int* hist, int16_t size, int min_value, int max_value, int bin_width)
{
    double variance = Variance(hist, size, min_value, max_value, bin_width);
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
    int8_t BIN_LEVEL = 10;
    int32_t MAX_SQUARE = 50;
    int16_t HIST_SIZE = BIN_LEVEL;
    int32_t NUM_SAMPLES = bmp.bmp_info_header.width*bmp.bmp_info_header.height;
    string binformat = BINFORMAT;


    int min_value = Min(bmp.data);
    int max_value = Max(bmp.data);
    int bin_width = ceil((max_value-min_value)/(double)BIN_LEVEL);
    int* hist = Histogram(bmp.data,bmp.bmp_info_header.width,bmp.bmp_info_header.height,BIN_LEVEL,min_value,max_value);
    int32_t square_count = Max_Element(hist,HIST_SIZE) / MAX_SQUARE;
    double mean_value = Mean(hist, HIST_SIZE, min_value, max_value, bin_width);
    double variance_value = Variance(hist, HIST_SIZE, min_value, max_value, bin_width);
    double std_value = Std(hist, HIST_SIZE, min_value, max_value, bin_width);

    cout << "# Num Samples : " << NUM_SAMPLES << endl;
    cout << "# Min : " << min_value << endl;
    cout << "# Max : " << max_value << endl;
    cout << "# Mean : " << mean_value << endl;
    cout << "# Variance : " << variance_value << endl;
    cout << "# Standard Deviation : " << std_value << endl;
    cout << "# each " << (char)254u <<" represents a count of " << square_count << endl;
    cout << "# --------------------------------------" << endl;

    for(int i=0; i< BIN_LEVEL; i++){
        printf("[%12.6f, %12.6f) |%9u| ", min_value+i*(double)bin_width, min_value+(i+1)*(double)bin_width, hist[i]);
        for(int isquare=0; isquare<hist[i]/square_count;isquare++) cout << (char)254u << " ";

        cout << endl;
    }

    cout << "# --------------------------------------" << endl;

    return 0;
}
