///#include <iostream>
#include "freader.h"
///#include <fstream>

using namespace std;
/**
BLOCK MASS - PDG code - particle (oznaczenia string)

DECAY - PDG - BR - NDA - ID1 - ID2 - (ID3)

PDG - nr cz¹steczki
BR - prawdopodobieñstwo rozpadu
NDA - na ile cz¹steczek siê rozpada
*/
char BRchar[15];///stringowy zapis BR
char linia[100];///1 linia zczytanego tekstu
int PDG=0;
double BR=0.0;
char inputplik[50],outputplik[50],graphplik[50];///nazwy plików I/O
bool wszystkie=1;///czy wyœwietliæ wszystko, co siê da, czy bardziej konkretnie
bool wyjscie=0;///czy zdefiniowany zosta³ plik wyjœcia
bool grapha=0;///czy jest wyjscie na grafikê
bool inputbyl=0;///upewnienie siê, czy zosta³ podany plik z danymi
ifstream wczyt,wczyt2;///odczytanie danych
ofstream zapis;///zapisanie danych do pliku tekstowego

int char_to_int(char znak)///zamiana charów na liczby
{
    switch(znak)
    {
        case '0':return 0;break;
        case '1':return 1;break;
        case '2':return 2;break;
        case '3':return 3;break;
        case '4':return 4;break;
        case '5':return 5;break;
        case '6':return 6;break;
        case '7':return 7;break;
        case '8':return 8;break;
        case '9':return 9;break;
        case '-':return -1;break;
        case '.':return -3;break;
        case ',':return -3;break;
        default:return -2;break;
    }
}

double char_to_br()///zamiana zapisu potêgi E na normaln¹ liczbê
{
    bool do_potegi=0,ujem=0;
    int po_przecinku=0,potega=0;
    double wynik=0,liczba;
    for(short int i=0;i<15;i++)
    {
        switch(BRchar[i])
        {
            case '.':po_przecinku=1;break;
            case ',':po_przecinku=1;break;
            case 'E':do_potegi=1;break;
            case '-':ujem=1;break;
            case '+':ujem=0;break;
            default:
                liczba=char_to_int(BRchar[i]);
                if(liczba!=-2)
                {
                    if(do_potegi==1)
                    {
                        potega=(potega*10)+liczba;
                    }
                    else
                    {
                        if(po_przecinku>0)
                        {

                            for(short int j=0;j<po_przecinku;j++){liczba*=0.1;}
                            wynik+=liczba;po_przecinku++;
                        }
                        else{wynik=(wynik*10)+liczba;}
                    }
                }
                break;
        }
    }
    if(potega>0)
    {
        for(short int j=0;j<potega;j++){wynik*=10;}
        if(ujem==1){wynik=1/wynik;}
    }
    return wynik;
}

void rozpad(int PDGid, double prawd, int poziom_rozpadu)///tworzenie rozpadów
{
    int ile_rozpadow=0;///na ile wersji mo¿e siê rozpaœæ
    char PDGchar[10];
    int PDGpor;
    //cout<<"Rozpadam "<<PDGid<<"!\n";
    ///pobranie danych
    wczyt2.open("decays.txt");
    while(!wczyt2.eof())
    {
        for(short int i=0;i<10;i++){PDGchar[i]='\0';}
        wczyt2.getline(linia,100);
        short int nrc=0,liczba;
        do
        {
            PDGchar[nrc]=linia[nrc];
            nrc++;
        }while(linia[nrc]!='#'&&linia[nrc]!='\0'&&nrc<100);
        ///zamiana na int
        PDGpor=0;
        for(short int j=0;j<nrc;j++)
        {
            liczba=char_to_int(PDGchar[j]);
            PDGpor*=10;PDGpor+=liczba;
        }
        if(PDGid==PDGpor||PDGpor==PDGid*(-1)){ile_rozpadow++;}
    }
    wczyt2.close();wczyt2.clear();
    //cout<<"Liczba rozpadow: "<<ile_rozpadow<<"!\n";
    if(ile_rozpadow>0)
    {
        int NDA[ile_rozpadow];///na ile cz¹stek siê rozpada
        double BRp[ile_rozpadow],prawd2;
        bool PDGjest[ile_rozpadow];
        int PDGnowe[ile_rozpadow][10];///nowe PDG po rozpadzie
        for(short int i=0;i<ile_rozpadow;i++){PDGjest[i]=0;for(short int j=0;j<10;j++){PDGnowe[i][j]=0;}}
        wczyt2.open("decays.txt");
        short int nrcw=0;
        while(!wczyt2.eof())
        {
            wczyt2.getline(linia,100);
            short int nrc=0,k;///nrc - numer linii w tekœcie, nrcw - numer rozpadu
            do
            {
                PDGchar[nrc]=linia[nrc];nrc++;
            }while(linia[nrc]!='#'&&linia[nrc]!='\0'&&nrc<100);
            ///zamiana na int
            PDGpor=0;
            for(short int j=0;j<nrc;j++)
            {
                k=char_to_int(PDGchar[j]);
                PDGpor*=10;PDGpor+=k;
            }
            if(PDGid==PDGpor||PDGpor==PDGid*(-1))
            {
                short int liczba;
                bool znak=0;
                k=0;nrc++;
                do{BRchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);///pobranie BR
                BRp[nrcw]=char_to_br();///zamiana BR na int
                nrc++;
                NDA[nrcw]=char_to_int(linia[nrc]);nrc+=2;///pobranie NDA
                for(short int i=0;i<NDA[nrcw];i++)
                {
					PDGnowe[nrcw][i]=0;
					k=0;do{PDGchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);nrc++;
					for(short int j=0;j<k;j++)
                    {
                        liczba=char_to_int(PDGchar[j]);
                        if(liczba>=0){PDGnowe[nrcw][i]*=10;PDGnowe[nrcw][i]+=liczba;}
                        else{znak=1;}
                    }
                    if(znak==1){PDGnowe[nrcw][i]*=(-1);}
                    if(PDGid<0){PDGnowe[nrcw][i]*=(-1);}
                }
                PDGjest[nrcw]=1;
                //cout<<"nr"<<nrcw<<" NDA: "<<NDA[nrcw]<<", BR: "<<BRp[nrcw];
                //for(short int i=0;i<NDA[nrcw];i++){cout<<", P("<<i<<"): "<<PDGnowe[nrcw][i];}
                //cout<<endl;
                nrcw++;
            }
        }
        wczyt2.close();wczyt2.clear();
        for(short int j=0;j<ile_rozpadow;j++)
        {
            prawd2=prawd;
            if(PDGjest[j]==1)
            {
                prawd2*=BRp[j];
                if(prawd2>BR)
                {
                    for(int i=0;i<poziom_rozpadu;i++){cout<<"   ";}
                    cout<<PDGid<<" -> ";
                    for(short int i=0;i<NDA[j];i++){cout<<PDGnowe[j][i]<<" ";}
                    cout<<", BR = "<<prawd2<<endl;
                    if(wyjscie==1)///output
					{
						for(int i=0;i<poziom_rozpadu;i++){zapis<<"   ";}
						zapis<<PDGid<<" -> ";
                    	for(short int i=0;i<NDA[j];i++){zapis<<PDGnowe[j][i]<<" ";}
                    	zapis<<", BR = "<<prawd2<<endl;
                    }
                    for(short int i=0;i<NDA[j];i++)
                    {
                        rozpad(PDGnowe[j][i],prawd2,poziom_rozpadu+1);
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv)///pobieranie parametrów
{
	for(int i=1;i<argc;i++)
    {
        if(argv[i][0]=='-'&&argv[i][1]=='-')
        {
            if(argv[i][2]=='P'&&argv[i][3]=='D'&&argv[i][4]=='G')
            {
                int j=5,liczba=-2;bool znak=1;
                while(argv[i][j]!='\0')
                {
                    liczba=char_to_int(argv[i][j]);
                    switch(liczba)
                    {
                        case -2:break;
                        case -1:znak=0;break;
                        default:if(znak!=1){liczba*=-1;}PDG*=10;PDG+=liczba;break;
                    }
                    j++;
                }
                wszystkie=0;
                cout<<"PDG = "<<PDG<<"\n";///wyœwietlenie
            }
            if(argv[i][2]=='B'&&argv[i][3]=='R')
            {
                int j=4,liczba=-2,ld=1;bool znak=0;
                ///double liczbad=0.0;
                while(argv[i][j]!='\0')
                {
                    liczba=char_to_int(argv[i][j]);
                    switch(liczba)
                    {
                        case -3:znak=1;break;///przecinek
                        case -2:break;
                        case -1:break;
                        default:if(znak==1){ld*=10;}BR*=10;BR+=liczba;break;
                    }
                    j++;
                }
                BR/=ld;
                cout<<"BR = "<<BR<<"\n";///wyœwietlenie
            }
            if(argv[i][2]=='i'&&argv[i][3]=='n'&&argv[i][4]=='p'&&argv[i][5]=='u'&&argv[i][6]=='t')
            {
                int j=8,k=0;
                do
                {
                    inputplik[k]=argv[i][j];
                    j++;k++;
                }while(argv[i][j]!='\0');
                inputbyl=1;
                cout<<"input = "<<inputplik<<"\n";///wyœwietlenie
            }
            if(argv[i][2]=='o'&&argv[i][3]=='u'&&argv[i][4]=='t'&&argv[i][5]=='p'&&argv[i][6]=='u'&&argv[i][7]=='t')
            {
                int j=9,k=0;
                do
                {
                    outputplik[k]=argv[i][j];
                    j++;k++;
                }while(argv[i][j]!='\0');
                wyjscie=1;
                cout<<"output = "<<outputplik<<"\n";///wyœwietlenie
                zapis.open(outputplik);
            }
            if(argv[i][2]=='g'&&argv[i][3]=='r'&&argv[i][4]=='a'&&argv[i][5]=='p'&&argv[i][6]=='h')
            {
                int j=8,k=0;
                do
                {
                    graphplik[k]=argv[i][j];
                    j++;k++;
                }while(argv[i][j]!='\0');
                grapha=1;
                cout<<"graphplik = "<<graphplik<<"\n";///wyœwietlenie
            }
        }
        ///cout<<argv[0]<<"\n"<<argv[1]<<"\n"<<argv[2]<<"\n";
    }
    if(inputbyl!=1){cout<<"Nie mozna wykonac programu, gdyz nie podano pliku z danymi!\nNastepnym razem wpisz --input= i podaj nazwe pliku!";return 0;}
    freader(inputplik);///przygotowanie plików pomocniczych
    wczyt.open("CMN.txt");
    if(wszystkie!=1){rozpad(PDG,1,0);}
    else
    {
        while(!wczyt.eof())///edit, odwo³ujemy siê do pliku z numerami PDG
        {
            ///pobranie numeru PDG
            ///cout<<"Pobieram numer PDG!\n";
			PDG=0;
            wczyt.getline(linia,100);
            short int i=0,liczba;
            do
            {
                liczba=char_to_int(linia[i]);
                PDG*=10;PDG+=liczba;
                i++;
            }while(linia[i]!='#'&&linia[i]!='\0'&&i<100);
            ///rozpad
            if(PDG>0)
            {
                ///cout<<PDG<<" ";
                rozpad(PDG,1,0);
            }
        }
    }
    zapis.close();
    wczyt.close();
    wczyt2.close();
	/**
	argc - liczba argumentów
	argv[0] - nazwa programu
	argv[1] - parametr 1...
	*/
	return 0;
}
