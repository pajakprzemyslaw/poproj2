//POP 2017 01 25 projekt 2 Pajπk Przemys≥aw EiT 165242 kolorowanie bitmapy z uzyciem pliku sonar szary 2
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
//struktura, ktora przechowuje informacje o naglowku pliku

struct BITMAPFILEHEADER
{
   int bfType;
   int bfSize;
   int bfReserved1;
   int bfReserved2;
   int bfOffBits;
};
//struktura ktora przechowuje informacje o naglowku obrazu

struct BITMAPINFOHEADER
{
   int biSize;
   int biWidth;
   int biHeight;
   int biPlanes;
   int biBitCount;
   int biCompression;
   int biSizeImage;
   int biXpelsPerMeter;
   int biYpelsPerMeter;
   int biCrlUses;
   int biCrlImportant;
};
//struktura kolorow obrazka bmp
struct COLOR
{
   char R;
   char G;
   char B;
};

void READ_HEADER(fstream &picture,BITMAPFILEHEADER &picture1);
void READ_PICTURE_HEADER(fstream &picture,BITMAPINFOHEADER &picture1);
int COUNT1(int n);
int COUNT2(int  n);
void COPY_HEADER(fstream &picture_in,fstream &picture_out, BITMAPINFOHEADER picture1, BITMAPFILEHEADER picture2);
void COPY_PICTURE(fstream &picture_in,fstream &picture_out, BITMAPINFOHEADER picture1, BITMAPFILEHEADER picture2, int funkcja1(int x), int funkcja2(int x));
void wyswietlFILEHEADER(BITMAPFILEHEADER x);
void wyswietlINFOHEADER(BITMAPINFOHEADER x);

int main()
{
    fstream picture,picture_out ;
    BITMAPFILEHEADER FileInfo;
    BITMAPINFOHEADER PictureInfo;

    picture.open("sonar_aktywny_szary.bmp", ios::in | ios::binary);
    if(!picture)
    {
        cout<<"Nie mozna otworzyc pliku!"<<endl;
        return 0;
    }

    READ_HEADER(picture,FileInfo);
    READ_PICTURE_HEADER(picture,PictureInfo);

    picture_out.open("sonar_aktywny_kolorowy.bmp", ios::out | ios::binary);
    if(!picture_out)
    {
        cout<<"Nie mozna otworzyc pliku!"<<endl;
        return 0;
    }
	//skopiuj oryginalny obraz
    COPY_HEADER(picture,picture_out,PictureInfo,FileInfo);
    //skopiuj oryginalny obraz
    COPY_PICTURE(picture,picture_out,PictureInfo,FileInfo, COUNT1, COUNT2);

    picture.close();
    picture_out.close();

    wyswietlFILEHEADER(FileInfo);
    cout << endl << endl;
    wyswietlINFOHEADER(PictureInfo);

    return 0;
}

void READ_HEADER(fstream &picture,BITMAPFILEHEADER &picture1)
{
	/*
	Funkcja czytajaca naglowek pliku, informacje sa przechowywane w naglowku pliku, 
	podajemy adres zmiennej int i  program go traktuje jako wskaünik na typ char, rzutuje na typ char to co znajduje sie w ()
	*/

    picture.read(reinterpret_cast<char*>(&picture1.bfType), 2);
    picture.read(reinterpret_cast<char*>(&picture1.bfSize), 4);
    picture.read(reinterpret_cast<char*>(&picture1.bfReserved1), 2);
    picture.read(reinterpret_cast<char*>(&picture1.bfReserved2), 2);
    picture.read(reinterpret_cast<char*>(&picture1.bfOffBits), 4);

}


void READ_PICTURE_HEADER(fstream &picture,BITMAPINFOHEADER &picture1)
{

    picture.read(reinterpret_cast<char*>(&picture1.biSize), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biWidth ), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biHeight), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biPlanes), 2);
    picture.read(reinterpret_cast<char*>(&picture1.biBitCount), 2);
    picture.read(reinterpret_cast<char*>(&picture1.biCompression), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biSizeImage), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biXpelsPerMeter), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biYpelsPerMeter), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biCrlUses), 4);
    picture.read(reinterpret_cast<char*>(&picture1.biCrlImportant), 4);

}

int COUNT1(int n)
/*
Funkcja sluzaca do obliczania wartosci kolorow(dodaje)
*/
{
    int wartosc = 0;

    for(int i=n ;; n--)
    {
        wartosc +=5;

        if(n == 52 || n == 154)
            break;
    }

    return wartosc;
}

int COUNT2(int  n)
{
	/*
	Funkcja sluzaca do obliczania wartosci kolorow(odejmuje)
	*/
    int wartosc = 255;

    for(int i=n;; n--)
    {
        wartosc -=5;

        if(n == 1 || n == 103 || n == 205)
            break;
    }

    return wartosc;
}

void COPY_HEADER(fstream &picture_in,fstream &picture_out, BITMAPINFOHEADER picture1, BITMAPFILEHEADER picture2)
{
	
	/*
	ustawienie wskaünika odczytu i zapisu danych na poczatek pliku;
	*/
	
    picture_in.seekg(0);
    picture_out.seekp(0);
    
    /*
    zmienna do przechowywania aktualnej wartosci bajtu, char bo zajmuje dokladnie jeden bajt
    */

	char bufor;

    
    
    for(int i = 0; i < picture2.bfOffBits; i++)
    {
        if(picture_in.get(bufor))
        {
            picture_out.put(bufor);
        }
        else
            cout << "Blad przepisywania naglowka" << endl;
    }
}

void COPY_PICTURE(fstream &picture_in,fstream &picture_out, BITMAPINFOHEADER picture1, BITMAPFILEHEADER picture2, int funkcja1(int x), int funkcja2(int x))
{
	
    //obliczenie bitÛw zerowych ktore zaokraglaja wartosc
	int bityZerowe = (picture1.biSizeImage - picture1.biWidth * picture1.biHeight * 3) / picture1.biHeight;
   
    cout << "bity zerowe: " << bityZerowe << endl;

    //ustawiamy pozycje wzkaznika czytania i pisania na pozycje na bfoffbits bo sekcja naglowka jest od 0 do bfoffbits
    picture_in.seekg(picture2.bfOffBits);
    picture_out.seekp(picture2.bfOffBits);

	char bufor;

    //zapis kolorowego obrazka
    /*
    petla najpierw iteruje po wierszach, nastepnie wchodzi do kazdej kolumny wiersza i zczytuje wartosc kazdego piksela(w bitmapach 3)
    */
    for(int i = 0; i < picture1.biHeight ; i++)
    {
        //przelec po wszystkich kolumnach
        for(int j = 0; j < picture1.biWidth * 3; j++)
        {

            //odczytaj bajt
            if(picture_in.get(bufor))
            {
                //ustal kolor, kolory sa zapisane w kolejnosci B, G, R
                int COLOR = j % 3;
                /*
				kolory sa w kolejnosci BGR, wiec przy dzieleniu przez 3 
				blue jest pierwszy(reszty0)
				green jest drugi (reszty1)
				red jest trzeci (reszty2)
                
                bufor sluzy do przechowywania akutalnie odczytanego bajtu
                
                unsigned char, bo przyjmuje wartosci od 0 do 255
                */
                
                int x = (unsigned char)bufor;
                
                if(x==0)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(255);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(0);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(255);
	                        break;
	                }
            	}
            	else if(x>0 && x<=50)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(255);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(0);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(COUNT2(x));
	                        break;
	                }
            	}
            	else if(x==51)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(255);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(0);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(0);
	                        break;
	                }
            	}
            	else if(x>51 && x<=101)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(255);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(COUNT1(x));
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(0);
	                        break;
	                }
            	}
            	else if(x==102)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(255);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(255);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(0);
	                        break;
	                }
            	}
            	else if(x>=102 && x<=152)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(COUNT2(x));
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(255);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(0);
	                        break;
	                }
            	}
            	else if(x==153)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(0);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(255);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(0);
	                        break;
	                }
            	}
            	else if(x>=153 && x<=203)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(0);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(255);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(COUNT1(x));
	                        break;
	                }
            	}
            	else if(x==204)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(0);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(255);
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(255);
	                        break;
	                }
            	}
            	else if(x>204 && x<=255)
				{
					switch(COLOR)
	                {
	                    //kolor B
	                    case 0:
	                        picture_out.put(0);
	                        break;
	                    //kolor G
	                    case 1:
	                        picture_out.put(COUNT2(x));
	                        break;
	                    //kolor R
	                    case 2:
	                        picture_out.put(255);
	                        break;
	                }
            	}
	   
 
            }
            else
               cout << "Blad kolorowania obrazu" << endl;
        }

        /*
        Dopisanie zerowych bajtow do kazego wiersza wyjsciowego obrazu, zeby nie byl znieksztalcony
        */
       for(int j = 0; j < bityZerowe; j++)
        {
            //przepisz bity
            if(picture_in.get(bufor))
            {
                picture_out.put(bufor);
            }
            else
                cout << "Blad przy dopisywaniu zerowych bajtow" << endl;
        }
    }

}

void wyswietlFILEHEADER(BITMAPFILEHEADER x)
{
    cout << setw(25) << "NAGLOWEK PLIKU" << endl << endl;
    cout << setw(20) << "bfType: " << x.bfType << endl;
    cout << setw(20) << "bfSize: " << x.bfSize << endl;
    cout << setw(20) << "bfReserved1: " << x.bfReserved1 << endl;
    cout << setw(20) << "bfReserved2: " << x.bfReserved2 << endl;
    cout << setw(20) << "bfOffBits: " << x.bfOffBits << endl;
}

void wyswietlINFOHEADER(BITMAPINFOHEADER x)
{
    cout << setw(25) << "NAGLOWEK OBRAZU" << endl<<endl;
    cout << setw(20) << "biSize: " << x.biSize << endl;
    cout << setw(20) << "biWidth: " << x.biWidth << endl;
    cout << setw(20) << "biHeight: " << x.biHeight << endl;
    cout << setw(20) << "biPlanes: " << x.biPlanes << endl;
    cout << setw(20) << "biBitCount: " << x.biBitCount << endl;
    cout << setw(20) << "biCompression: " << x.biCompression << endl;
    cout << setw(20) << "biSizeImage: " << x.biSizeImage << endl;
    cout << setw(20) << "biXpelsPerMeter: " << x.biXpelsPerMeter << endl;
    cout << setw(20) << "biYpelsPerMeter: " << x.biYpelsPerMeter << endl;
    cout << setw(20) << "biCrlUses: " << x.biCrlUses << endl;
    cout << setw(20) << "biCrlImportant: " << x.biCrlImportant << endl;
}
