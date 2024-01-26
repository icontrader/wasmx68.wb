#include <math.h>

double trunc(double x)
{
    if (x < 0)
    {
        return ceil(x);
    }
    else
    {
        return floor(x);
    }
}


float sqrtf(float x)
{
    return (float)sqrt((double)x);
}
float truncf(float x)
{
    return (float)trunc((double)x);
}
float floorf(float x)
{
    return (float)floor((double)x);
}
float ceilf(float x)
{
    return (float)ceil((double)x);
}
