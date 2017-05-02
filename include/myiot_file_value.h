/*
 * myiot_file_value.h
 *
 *  Created on: 23.04.2017
 *      Author: a4711
 */

#ifndef MYIOT_FILE_VALUE_H_
#define MYIOT_FILE_VALUE_H_

#include <string>
#include <unistd.h>
namespace MyIOT
{
  /// read a single value (int, long, or string) from a file. Your platform has to have a file system.
  class FileValue
  {
     public:
       FileValue(){}
       FileValue(const char* file_name):fname(file_name){}

       void set_file_name(const char* file_name)
       {
         fname = file_name;
       }

       const char* get_file_name() const
       {
         return fname.c_str();
       }

       int readInt(int defaultValue = 0)
       {
            FILE* fp = fopen(fname.c_str(), "r");
            if (!fp)
              return defaultValue;
            char buffer[100];
            fgets(buffer, (int) sizeof(buffer), fp);
            fclose(fp);
            return ::atoi(buffer);
       }

       bool writeInt(int val)
       {
           FILE* fp = fopen(fname.c_str(), "w+");
           if (!fp)
             return false;
           char buffer[100];
           sprintf(buffer,"%d",val);
           fputs(buffer, fp);
           fclose(fp);
           return true;
       }

       int readLong(long defaultValue = 0)
       {
            FILE* fp = fopen(fname.c_str(), "r");
            if (!fp)
              return defaultValue;
            char buffer[100];
            fgets(buffer, (int) sizeof(buffer), fp);
            fclose(fp);
            return ::atol(buffer);
       }

       bool writeLong(long val)
       {
           FILE* fp = fopen(fname.c_str(), "w+");
           if (!fp)
             return false;
           char buffer[100];
           sprintf(buffer,"%ld",val);
           fputs(buffer, fp);
           fclose(fp);
           return true;
       }

       void readString(std::string& value)
       {
            FILE* fp = fopen(fname.c_str(), "r");
            if (!fp)
              return;
            char buffer[1024];
            fgets(buffer, (int) sizeof(buffer), fp);
            fclose(fp);
            value = buffer;
       }

       bool writeString(const char* str)
       {
           FILE* fp = fopen(fname.c_str(), "w+");
           if (!fp)
             return false;
           fputs(str, fp);
           fclose(fp);
           return true;
       }

       bool delete_file()
       {
    	   return 0 == unlink(fname.c_str());
       }

    private:
      std::string fname;
  };
}

#endif /* MYIOT_FILE_VALUE_H_ */
