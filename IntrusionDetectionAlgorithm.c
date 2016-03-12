/*************************************/
/* CONSTANTS USED IN THE ALGORITHM */
#define ANALOG_DET_WNDW 128
/* =N, size of window to compute Haar transform. Should be a power of 2 */
#define BINS 7
/* BINS = log2(ANALOG_DET_WNDW), number of frequency bins */
#define TEMP_WINDOW 16
/* =L, Number of samples to slide. Should be a power of 2 */
#define AVERAGE_2 306
/* 2*DC(=2450) / 2^SAMPLE_NORMALISE */
#define GAMMA_THRESHOLD 0
/* the projection value is compared against this */
#define SAMPLE_NORMALISE 4
/* the sample value from sensor is normalised by 2^SAMPLE_NORMALISE */
#define GAMMA_NORMALISE 65536
/* 4096^2 / 2^( 2 * SAMPLE_NORMALISE ) */
#define CHANNELS 3
/* Number of sensor channels */
#define SUPRESS_THRESHOLD 0.0
/* to declare a new intrusion (to supress multiple successive decisions), the
 * projected value should have gone below this threshold */
#define NORMALISE ( GAMMA_NORMALISE * ANALOG_DET_WNDW )
/* Besides normalisation of the sample by 4096, this constant also
 * normalises the squared (unnormalised) HT output by ANALOG_DET_WNDW */
/*************************************/
/* HYPERPLANE PARAMETERS */
float W[]={-398.0744,-13.1683,-50.5246,-52.7896,-31.7128,-207.1104,790.8064,
21296.1216};
/* In this W, normalisation of the HT output is taken care*/
float B = -1.0538;
/*************************************/
/* functions and variables used in INTRUSION DETECTION ALGORITHM */
bool firstTime[ CHANNELS ] = { true, true, true };
int sampleArray[ CHANNELS ][ ANALOG_DET_WNDW ];
int circ[ CHANNELS ] = { 0, 0, 0 };
int sampleIndex[ CHANNELS ] = { 0, 0, 0 };
bool supressFlag[ CHANNELS ] = { true, true, true };
int Classify( int );
int Analog_Classify( int, int );
/*************************************/
int Analog_Classify( int data, int channel ) /* channel is 0,1,2,... */
{
    int cmp = TEMP_WINDOW, ledon = 2;
    /* ledon = 0 -> Clutter, 1 -> Intruder, 2 -> No decision */
    sampleArray[ channel ][ circ[ channel ] ] = ( data >> SAMPLE_NORMALISE );
    circ[ channel ]++;
    /* circ[channel] indicates current position of the sample in the array */
    sampleIndex[ channel ]++;
    /* sampleIndex[channel] indicates the number of samples collected for the
next computation */
    /* clearing up the pointers */
    if( circ[ channel ] == ANALOG_DET_WNDW )
    {
        circ[ channel ] = 0;
        /* wrapping around the array for filling up new samples from the beginning
of the array*/
    }
    if( firstTime[ channel ] )
        /* buffering 'ANALOG_DET_WNDW' number of samples for the first time,
thereafter 'TEMP_WINDOW' number of samples */
    {
        cmp = ANALOG_DET_WNDW;
    }
    if( sampleIndex[ channel ] == cmp )
        /* Indicates that buffer is filled with valid data */
    {
        sampleIndex[ channel ] = 0;
    }
    /* calling the detection algorithm */
    if( sampleIndex[ channel ] == 0 )
    {
        ledon = Classify( channel );
        /* Valid samples in tha array start from circ[channel] wrapping
back at the end of the array and ends at circ[channel]-1 */
        firstTime[ channel ] = false;
    }
    return ledon;
}
int Classify( int channel )
{
    int i = 0, j = 0, k = 0, l = 0, m = 0, p = 0, n = 0, x = 0, y = 0, Lgj = 0,
            decision = 0;
    int HT[ ANALOG_DET_WNDW ], sum = 0, diff = 0;
    float binsqr[ BINS+1 ];
    /* binsqr size should be log2(N)+1 */
    float bval = B;
    /* Fast Haar Transform cum Frequency Binning. Output (HT[]) is permuted.
Binning is done simulatneously in binsqr[] */
    i = 1;
    /* 0.5 * the width of Haar wavelet in each bin */
    j = 2;
    /* the width of Haar wavelet in each bin */
    m = ANALOG_DET_WNDW;
    Lgj = 0;
    for( l = BINS; l > 0; l-- )
        /* Bins are computed from BINS to 1 */
    {
        binsqr[l] = 0.0;
        m = m >> 1;
        /* Number of components in each bin */
        Lgj++;
        for( k = 0; k < m; k++ )
        {
            p = k << Lgj;
            /* time offset for each component in a bin */
            if( l == BINS )
                /* only for the first stage in FHT */
            {
                n = p + circ[ channel ];
                /* updating the starting position of valid samples in the
array */
                x = n;
                y = n + i;
                if( n >= ANALOG_DET_WNDW )
                {
                    x = n - ANALOG_DET_WNDW;
                    /* wrapping around the array */
                }
                if( ( n + i ) >= ANALOG_DET_WNDW )
                {
                    y = n + i - ANALOG_DET_WNDW;
                    /* wrapping around the array */
                }
                sum=sampleArray[channel][x]+sampleArray[channel][y]-(AVERAGE_2);
                diff=sampleArray[channel][x]-sampleArray[channel][y];
            }
            else
            {
                sum = HT[ p ] + HT[ p + i ];
                diff = HT[ p ] - HT[ p + i ];
            }
            HT[ p ] = sum;
            HT[ p + i ] = diff;
            binsqr[ l ] = binsqr[ l ] + (float) HT[ p+i ] * (float) HT[ p+i ];
        }i
                = j;
        j = j << 1;
    }
    binsqr[ 0 ] = ( float ) HT[ 0 ] * ( float ) HT[ 0 ];
    /* gamma=w'x+b with normalisation */
    for( i = 0; i <= BINS; i++ )
    {
        bval = bval + ( W[ i ] * binsqr[ i ] / NORMALISE );
    }
    /* to supress independently the multiple successive decisions of the 3
channels */
    if( !supressFlag[ channel ] )
    {
        if( bval >= GAMMA_THRESHOLD )
        {
            supressFlag[ channel ] = true;
            decision = 1;
        }
    }
    else
    {
        if( bval < SUPRESS_THRESHOLD )
        {
            supressFlag[ channel ] = false;
        }
    }
    return decision;
}
