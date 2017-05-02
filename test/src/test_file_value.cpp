#include <gtest/gtest.h>

#include "../../include/myiot_file_value.h"

class T_File_Value : public ::testing::Test
{
  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

using namespace MyIOT;

TEST_F(T_File_Value, a_FileValue_has_a_file_name)
{
  FileValue f1;
  std::string fname1 = "hello world";
  f1.set_file_name(fname1.c_str());
  std::string fname2 = f1.get_file_name();
  ASSERT_EQ(fname1,fname2);
}

TEST_F(T_File_Value, file_name_can_be_set_by_constructor)
{
  FileValue f1("Hello World");
  ASSERT_EQ(std::string("Hello World"), std::string(f1.get_file_name()));
}

TEST_F(T_File_Value, reading_non_existing_file_returns_default_value)
{
  FileValue f1("D:\\temp\\non_existing_file");
  int ret = f1.readInt(42);
  ASSERT_EQ(42,ret);
}

TEST_F(T_File_Value, writing_to_a_fileValue_creates_a_file)
{
	FileValue f1("D:\\temp\\newfile");
	ASSERT_TRUE(f1.writeInt(42));

	FILE* fp = fopen(f1.get_file_name(), "r");
	ASSERT_NE(nullptr, fp);
	if (fp) fclose(fp);

	ASSERT_TRUE(f1.delete_file());

	fp = fopen(f1.get_file_name(), "r");
	ASSERT_EQ(nullptr, fp);
}

TEST_F(T_File_Value, write_and_read_some_ints)
{
	FileValue f1("D:\\temp\\newfile");
	FileValue f2("D:\\temp\\newfile");

	for (int i = 0; i < 10; i++)
	{
		f1.writeInt(i);
		ASSERT_EQ(i, f2.readInt(-1));
	}
}

TEST_F(T_File_Value, write_and_read_some_longs)
{
	FileValue f1("D:\\temp\\newfile");
	FileValue f2("D:\\temp\\newfile");

	for (long i = 0; i < 10; i++)
	{
		f1.writeLong(i);
		ASSERT_EQ(i, f2.readLong(-1));
	}

	f1.delete_file();
}

TEST_F(T_File_Value, write_and_read_some_strings)
{
	FileValue f1("D:\\temp\\newfile");
	FileValue f2("D:\\temp\\newfile");

	const char* texts[] =
	{
	  "one",
	  "two",
	  "three",
	  "four",
	  "five",
	  "six",
	  "seven",
	  "eight",
	  "nine",
	  "ten"
	};


	for (int i = 0; i < 10; i++)
	{
		f1.writeString(texts[i]);
		std::string val;
		f2.readString(val);
		ASSERT_EQ(std::string(texts[i]), val);
	}
	f1.delete_file();
}

