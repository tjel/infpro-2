#include <iostream>
///#include <fstream>

using namespace std;
/**
BLOCK MASS - PDG code - particle (oznaczenia string)

DECAY - PDG - BR - NDA - ID1 - ID2 - (ID3)

PDG - nr cz¹steczki
BR - prawdopodobieñstwo rozpadu
*/
char BRchar[15];
int PDG=0;
double BR=0.0;
int PDGciag[100];
char inputplik[50],outputplik[50],graphplik[50];
bool wszystkie=1;
bool wyjscie=0;
bool grapha=0;///czy jest wyjscie na grafikê

int char_to_int(char znak)
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

double char_to_br()
{
    bool znacznik=0,do_potegi=0,ujem=0;
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

void rozpad(int PDGid, double prawd)
{
    int ile_rozpadow=0;
    ///pobranie danych
    ile_rozpadow=1;///edit
    char BRpob[ile_rozpadow][15];///prawdopodobieñstwo rozpadu
    int NDA[ile_rozpadow];///na ile cz¹stek siê rozpada
    double BRp[ile_rozpadow],prawd2;
    for(short int j=0;j<ile_rozpadow;j++)
    {
        prawd2=prawd;
        NDA[j]=2;///edit
        for(short int i=0;i<15;i++){BRchar[i]=BRpob[j][i];}
        BRp[j]=char_to_br();
        prawd2*=BRp[j];
        if(prawd>BR)
        {
            int PDGpob[NDA[j]];
            for(short int i=0;i<NDA[j];i++)
            {
                ;///pobranie numerów PDG
            }

            for(short int i=0;i<NDA[j];i++)
            {
                ;///pobranie numerów PDG
                rozpad(PDGpob[i],prawd2);
            }
        }
    }
}

int main(int argc, char** argv)
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
	/**
	argc - liczba argumentów
	argv[0] - nazwa programu
	argv[1] - parametr 1...
	*/
	return 0;
}
