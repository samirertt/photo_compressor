#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class pgmfile
{
	string pgmtype;
	unsigned int width;
	unsigned int height;
	unsigned short max_graylevel;
	unsigned char* data;


public:
	pgmfile();
	pgmfile(const pgmfile& obj);
	pgmfile(const string& filename);
	~pgmfile();

	string get_pgmtype();
	void set_pgmtype(const string& pgmtype);
	unsigned int get_width();
	void set_width(unsigned int width);
	unsigned int get_height();
	void set_height(unsigned int heigth);
	unsigned short get_max_graylevel();
	void set_max_graylevel(unsigned short max_graylevel);
	void readpgmfile(const string& filename);
	void compresspgmfile();
	void decompressingfile();
	
};
int string2int(const string& str);

pgmfile::pgmfile()
{
	this->pgmtype = "";
	this->width = 0;
	this->height = 0;
	//this->max_graylevel = 0;
	this->data = {};
}

pgmfile::pgmfile(const pgmfile& obj)
{
	this->width = obj.width;
	this->height = obj.height;
	this->max_graylevel=obj.max_graylevel;

	this->data = new unsigned char[(this->width * this->height)];
	for (unsigned int i = 0; i < (this->width * this->height); i++)
	{
		this->data[i] = obj.data[i];
	}

}

pgmfile::pgmfile(const string& filename)
{
	readpgmfile(filename);
	compresspgmfile();

}
pgmfile::~pgmfile()
{
	delete[] this->data;
}
string pgmfile::get_pgmtype()
{
	return this->pgmtype;
}
void pgmfile::set_pgmtype(const string& pgmtype)
{
	this->pgmtype = pgmtype;
}
unsigned int pgmfile::get_width()
{
	return this->width;
}
void pgmfile::set_width(unsigned int width)
{
	this->width = width;
}
unsigned int pgmfile::get_height()
{
	return this->height;
}
void pgmfile::set_height(unsigned int height)
{
	this->height = height;
}
unsigned short pgmfile::get_max_graylevel()
{
	return this->max_graylevel;
}
void pgmfile::set_max_graylevel(unsigned short max_graylevel)
{
	this->max_graylevel = max_graylevel;
}


void pgmfile::readpgmfile(const string& filename) {
	ifstream handler(filename, ios::binary);

	if (handler.is_open())
	{
		// first lets see the pgm type P5 or P2
		getline(handler, this->pgmtype);
		cout << this->pgmtype << endl;

		if (pgmtype == "p5" || pgmtype == "P5") 
		{
			//in this part i am reading until line's first value is not #. ignoring the comment
			string line;
			getline(handler, line);
			while (line[0] == '#')
			{
				getline(handler, line);
			}
			string temp = "";

			//this this part i am getting the width and height by casting it to int from string.
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ') 
				{
					temp += line[i];
					
				}

				else
				{
					this->width = string2int(temp);
					temp = "";
				}
			}
			this->height = string2int(temp);

			cout << this->width << endl;
			cout << this->height << endl;

			// to get the maximum gray level
			handler >> this->max_graylevel;
			cout << this->max_graylevel << endl;

			// reading the rest of the data
			streampos datasize = this->width * this->height;
			this->data = new unsigned char[datasize];
			//char* not needed its already char*
			handler.read((char*)this->data, datasize);
			handler.close();
			cout << endl;

		}

		else if (pgmtype == "p2" || pgmtype == "P2") 
		{	//in this part i am reading until line's first value is not #. ignoring the comment
			string line;
			getline(handler, line);
			while (line[0] == '#') 
			{
				getline(handler, line);
			}
			string temp = "";
			//this this part i am getting the width and height by casting it to int from string.
			for (unsigned int i = 0; i < line.length(); i++) 
			{
				if (line[i] == ' ')
				{
					this->width = string2int(temp);
					temp = "";
				}
				else
				{
					temp += line[i];
				}
			}
			this->height = string2int(temp);

			cout << this->width << endl;
			cout << this->height << endl;

			handler >> this->max_graylevel;
			cout << this->max_graylevel << endl;

			unsigned int datasize = this->width * this->height;
			this->data = new unsigned char[datasize+1];

			//saving the data
			for (unsigned int i = 0; i < datasize; i++) 
			{
				//could have been unsigned char not int
				int p2_value;
				handler >> p2_value;
				this->data[i] = (unsigned char)p2_value;
			}

			handler.close();

		}
		else {
			cout << "cannot open the file:" << filename << endl;
		}
	}
}

void pgmfile::compresspgmfile()
{
	if(this->pgmtype == "P5" || this->pgmtype == "p5")
	{
		unsigned int datasize = this->width * this->height;
		//dynamically allocating memory to save the compressed data
		//i plused datasize by 1 because i used to get a warning on buffer overrun
		unsigned char* compressed_data_p5 = new unsigned char[datasize+1];
		//making the pivot at the beginning the first value
		unsigned char current_value = this->data[0];
		//started current_run from 1 because every value is there atleast ones
		unsigned int current_run = 1;
		unsigned int index = 0;

		for (unsigned int i = 1; i < datasize; i++)
		{
			if (this->data[i] == current_value)
			{
				//if the value is same with the current value I increase the amount of runs 
				current_run++;
			}
			else
			{
				//if its different I assign the number of repeatitions 
				compressed_data_p5[index++] = (unsigned char)current_run;
				//and then i assign he repeated value
				compressed_data_p5[index++] = current_value;
				//now the pivot is the next different value
				current_value = this->data[i];
				current_run = 1;
			}
		}
		//I make it one last time becasue the last value never gets in the else statement
		compressed_data_p5[index++] = (unsigned char)current_run;
		compressed_data_p5[index++] = current_value;




		//now am writing the data inside a file i create

		ofstream handler("compressed.se", ios::binary);
		if (handler.is_open())
		{
			handler.write((char*)(compressed_data_p5), index);
			cout << "compressed! file name: compressed.se"<<endl;
			handler.close();
		}
		else
		{
			cout << "Unable to open file for writing compressed data!" << endl;
		}

		delete[] compressed_data_p5;
	}
	else if(this->pgmtype == "P2" || this->pgmtype == "p2")
	{
		unsigned int datasize = this->width * this->height;
		//dynamically allocating memory to save the compressed data
		unsigned char* compressed_data_p2 = new unsigned char[datasize+1];
		//making the pivot at the beginning the first value
		unsigned char current_value = this->data[0];
		unsigned int current_run = 1;
		unsigned int index = 0;

		for (unsigned int i = 0; i < datasize; i++)
		{
			if (current_value == this->data[i])
			{
				current_run++;
			}
			else
			{
				//if its different I assign the number of repeatitions 
				compressed_data_p2[index++] = (unsigned char)current_run;
				//and then i assign he repeated value
				compressed_data_p2[index++] = current_value;
				//now the pivot is the next different value
				current_value = this->data[i];
				current_run = 1;
			}

		}
		//I make it one last time becasue the last value never gets in the else statement
		compressed_data_p2[index++] = (unsigned char)current_run;
		compressed_data_p2[index++] = current_value;


		ofstream handler("compressed_p2.se",ios::binary);
		if(handler.is_open())
		{
			handler.write((char*)compressed_data_p2, index);
			cout << "compressed! file name: compressed_p2.se" << endl;
		}
		else
		{
			cout << "file is unable to open :("<<endl;
		}
	}
}

void pgmfile::decompressingfile()
{
	if(this->pgmtype == "p2" || this->pgmtype == "P2")
	{
		ifstream handler("compressed_p2.se",ios::binary | ios::ate);
		if (handler.is_open())
		{
			//reading the compressed data and savign it to mblock
			unsigned int datasize = handler.tellg();
			unsigned char* mBlock = new unsigned char[datasize];
			handler.seekg(0, ios::beg);
			handler.read((char*)mBlock, datasize);
			handler.close();
			//dynamically allocating memory to save the decompressed data
			unsigned char* decompressed_data = new unsigned char[(this->width * this->height) + 1];
			//this would hold the number of times a value repeated
			unsigned int run_decompression = 0;
			//the value(pixel) which repeats 
			unsigned int value_decompression = 0;
			unsigned int k=0;
			unsigned int i = 0;

			//iterating i by 2 
			for (i = 0; i < datasize; i += 2)
			{
				//number of repeatation 
				run_decompression = mBlock[i];
				//next value is the repeated value
				value_decompression = mBlock[i + 1];
				//saving the value as much as it repeated in memory
				for (unsigned int j = 0; j < run_decompression; j++)
				{
					decompressed_data[k++] = value_decompression;
					
				}
			}
			//this is for the last 
			if (i == datasize - 1)
			{
				run_decompression = mBlock[i];
				value_decompression = mBlock[i];

				for (unsigned int i = 0; i < run_decompression; i++)
				{
					decompressed_data[k] = value_decompression;
				}
			}


			delete[] mBlock;
			//writing it back in a file
			ofstream handler("photo2_decompressed.pgm");

			if (handler.is_open())
			{
				//writing in the file

				handler << this->pgmtype << endl;
				handler << this->width << " ";
				handler << this->height << endl;
				handler << this->max_graylevel << endl;

				
				for (unsigned int i = 0; i < this->width * this->height; i++)
				{
					handler << (int)decompressed_data[i];
					handler << " ";
					
				}
				delete[] decompressed_data;
				cout << "Decompressed file 'photo2_decompressed.pgm' written successfully." << endl;
				handler.close();
			}
			
		}

	}
	else if (this->pgmtype == "p5" || this->pgmtype == "P5")
	{
		ifstream handler("compressed.se", ios::binary | ios::ate);
		if (handler.is_open())
		{
			
			unsigned int datasize = handler.tellg();
			unsigned char* mBlock = new unsigned char[datasize+1];
			handler.seekg(0, ios::beg);
			handler.read((char*)mBlock, datasize);
			handler.close();

			unsigned char* decompressed_data = new unsigned char[(this->width * this->height) + 1];
			unsigned int run_decompression = 0;
			unsigned int value_decompression = 0;
			unsigned int k = 0;
			unsigned int i = 0;

			for (i = 0; i < datasize; i += 2)
			{
				run_decompression = mBlock[i];
				value_decompression = mBlock[i + 1];

				for (unsigned int j = 0; j < run_decompression; j++)
				{
					decompressed_data[k++] = value_decompression;

				}
			}
			if (i == datasize - 1)
			{
				run_decompression = mBlock[i];
				value_decompression = mBlock[i];

				for (unsigned int i = 0; i < run_decompression; i++)
				{
					decompressed_data[k] = value_decompression;
				}
			}


			delete[] mBlock;

			ofstream handler("decompressed_p5.pgm", ios::binary);
			if (handler.is_open())
			{
				
				for (unsigned int i = 0; i < (this->width * this->height); i++)
				{
					handler.write((char*)&decompressed_data[i], sizeof(unsigned char));
				}
				delete[] decompressed_data;
				handler.close();
				cout << "Decompressed file 'decompressed_p5.pgm' written successfully." << endl;
			}
			else
			{
				cout << "was unable to write in the file" << endl;
			}

		}
	}
	else
	{
		cout << "wrong file type"<<endl;
	}
}
int string2int(const string& str)
{
	//in this function i change string to integer
	int ans = 0; 
	for (size_t i = 0; i < str.length(); i++)
	{
		ans *= 10; 
		ans += str[i] - '0'; 
	}
	return ans; 
}

