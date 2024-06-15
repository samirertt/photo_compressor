#include "Header.h"


int main()
{


	string filename = "";//photo_p2.pgm or photo_p5.pgm
	string decompressing="";

	cout << "enter file name: "<<endl;
	cin >> filename;

	pgmfile photo2(filename);

	cout << "do you want to decompress your file: Yes | No:";
	cin >> decompressing;

	if (decompressing == "yes" || decompressing == "Yes")
	{
		photo2.decompressingfile();
	}

	return 0;
}