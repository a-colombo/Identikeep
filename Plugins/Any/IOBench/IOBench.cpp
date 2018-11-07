#include <stdio.h>
#include "IOBench.h"
#include <cmath>
#include <time.h>
#include <iostream>
// Logging
#include "plog/Log.h"

#define MATCHALL match::list | match::node | match::world

#define FILENAME "IOBench.temp"
#define FSIZE 1024*1024*8
#define TRIES 16

using namespace std;

bool IOBench::Collect(int argc, char *argv[])
{
    
    clock_t tt;
    bool is_ok = true;

    float write_speed, write_speed_dev;
    int   write_speed_dev_perc;
    float write_sum=0;
    float write_ssum=0;

    float read_speed, read_speed_dev;
    int   read_speed_dev_perc;
    float read_sum=0;
    float read_ssum=0;
    
    
    LOG_VERBOSE << "Parsing " << this->Name() << " info...";
    
    FILE * pFile;
    char *buffer = new char[FSIZE];
    
    for(int i_tries=0; i_tries<TRIES; i_tries++){
        pFile = fopen (FILENAME, "wb");
        tt=clock();
        fwrite (buffer , sizeof(char), FSIZE, pFile);
        tt=clock()-tt;
        fclose (pFile);
        float temp_time= float(tt)/CLOCKS_PER_SEC;
        float temp_speed= float(FSIZE*sizeof(char))/(temp_time*1000000.);
        write_sum+=temp_speed;
        write_ssum+=temp_speed*temp_speed;
        
        pFile = fopen (FILENAME, "rb");
        tt=clock();       
        fread(buffer , sizeof(char), FSIZE, pFile);
        tt=clock()-tt;
        fclose (pFile);
        temp_time= float(tt)/CLOCKS_PER_SEC;
        temp_speed= float(FSIZE*sizeof(char))/(temp_time*1000000.);
        read_sum+=temp_speed;
        read_ssum+=temp_speed*temp_speed;
        
        remove(FILENAME);
        
    }
    
    delete [] buffer;
    
    write_speed= write_sum/float(TRIES);
    write_speed_dev=sqrt(write_ssum/float(TRIES)-write_speed*write_speed);
    write_speed_dev_perc=int(write_speed_dev/write_speed*100.+0.5);
   
    std::string title="iobenchWriteSpeed";
    std::string unit="MB/s (+/- "+std::to_string(write_speed_dev_perc)+"%) - Pack size: " +std::to_string(sizeof(char)*FSIZE/1000000.) + "MB";
    Item<float> iwrite = Item<float>(title, unit, MATCHALL, write_speed);
    m_items.floats.push_back(iwrite);
    
    
    
    read_speed= read_sum/float(TRIES);
    read_speed_dev=sqrt(read_ssum/float(TRIES)-read_speed*read_speed);
    read_speed_dev_perc=int(read_speed_dev/read_speed*100.+0.5);
   
    title="iobenchReadSpeed";
    unit="MB/s (+/- "+std::to_string(read_speed_dev_perc)+"%) - Pack size: " +std::to_string(sizeof(char)*FSIZE/1000000.) + "MB";
    Item<float> iread = Item<float>(title, unit, MATCHALL, read_speed);
    m_items.floats.push_back(iread);

    return is_ok;
};









