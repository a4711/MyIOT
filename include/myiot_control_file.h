/*
 * myiot_controlFile.h
 *
 *  Created on: 22.04.2017
 *      Author: a4711
 */

#ifndef MYIOT_CONTROL_FILE_H_
#define MYIOT_CONTROL_FILE_H_

#include <stddef.h>
#include <string.h>



namespace MyIOT
{
    /// Boolean flag based on the content of a file. (You will need a file system.)
    /* If the first byte in the file equals "1", the control ("get") returns true.
     */
	class ControlFile
	{
	public:
		ControlFile(const char* fname) :
				file_name(fname)
		{
		}

		bool get() const
		{
			FILE* fp = fopen(file_name, "r");
			if (!fp)
				return false;
			char buffer[100];
			fgets(buffer, (int) sizeof(buffer), fp);
			bool retval = 0 == strncmp(buffer, "1", 1);
			fclose(fp);
			return retval;
		}

	private:
		const char* file_name;
	};
}

#endif /* MYIOT_CONTROL_FILE_H_ */
