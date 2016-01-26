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
const int lPDGmax=100;///maksymalna liczba PDG, które mog¹ siê jakoœ rozpadaæ
const int nmax=100;///maksymalna liczba PDG, których ujemna wersja rozpada siê tak samo
const int rxmax=10,rymax=400;///maksymalna szerokoœæ i d³ugoœæ tabeli rozpadu
const int cxmax=100,cymax=400;///maksymalna szerokoœæ i d³ugoœæ tabeli ci¹gów odwo³añ do tabeli rozpadu
const int kmax=50;///maksymalna liczba kopii ci¹gu

char BRchar[15];///stringowy zapis BR
char linia[100];///1 linia zczytanego tekstu
int PDG=0;
int PDGrozpadane[lPDGmax];///które PDG maj¹ rozpady
int ilerozp=0;///ile cz¹steczek ma swój rozpad
double BR=0.0;
double BRact;///aktualna wartoœæ BR rozpadu
char inputplik[50],outputplik[50],graphplik[50];///nazwy plików I/O
bool wszystkie=1;///czy wyœwietliæ wszystko, co siê da, czy bardziej konkretnie
bool wyjscie=0;///czy zdefiniowany zosta³ plik wyjœcia
bool grapha=0;///czy jest wyjscie na grafikê
bool inputbyl=0;///upewnienie siê, czy zosta³ podany plik z danymi
int rciag[rxmax][rymax],rir[rxmax][rymax];
double rprawd[rymax],ciagBR[cymax];
int ciagPDG[cxmax][cymax];
int neutral[nmax];
int nrx,nry;
int ymax;///faktyczna d³ugoœæ tabeli rozpadu
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

bool jest_neutralny(int PDGid){for(short int i=0;i<nmax;i++){if(neutral[i]==PDGid||neutral[i]==PDGid*(-1)){return 1;}}return 0;}

/**void rozpad(int PDGid, double prawd, int poziom_rozpadu)///tworzenie rozpadów
{
    int ile_rozpadow=0;///na ile wersji mo¿e siê rozpaœæ
    char PDGchar[10];
    int PDGpor;
    int BRp;
    int nrciagx=0,nrciagy=0;
    cout<<"Rozpadam "<<PDGid<<"!\n";
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
        //short int k=0;nrc++;
        //do{BRchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);///pobranie BR
        //BRp=char_to_br();///zamiana BR na int
        //if(BRp>BR&&(PDGid==PDGpor||PDGpor==PDGid*(-1))){ile_rozpadow++;}
        if(PDGid==PDGpor||PDGpor==PDGid*(-1)){ile_rozpadow++;}
    }
    wczyt2.close();wczyt2.clear();
    cout<<"Liczba rozpadow: "<<ile_rozpadow<<"!\n";
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
                if(BRp[nrcw]>BR)
                {
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
                    PDGciag[nrciagy][nrciagx]=PDGid;BRciag[nrciagx]=prawd2;
                    bool jest_rozpadle;
                    for(short int i=0;i<NDA[j];i++)
                    {
                        jest_rozpadle=0;
                        for(short int ii=0;ii<lPDGmax;ii++){if(PDGnowe[j][i]==PDGrozpadane[ii]||PDGnowe[j][i]==PDGrozpadane[ii]*(-1)){jest_rozpadle=1;break;}}
                        if(jest_rozpadle==1)
                        {
                            short int xx=nrciagx,yy=nrciagy;
                            do{nrciagy++;}while(PDGciag[nrciagy][0]!=0);
                            for(short int ii=0;ii<nrciagx;ii++){PDGciag[nrciagy][ii]=PDGciag[yy][ii];}
                            rozpad(PDGnowe[j][i],prawd2,poziom_rozpadu+1);
                            nrciagx=xx;nrciagy=yy;
                        }
                    }
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
}*/

/**void rozpad_ciagi(int PDGid, short int x, short int y, double BRa)
{
    bool jest=0;
    for(short int i=0;i<lPDGmax;i++)
    {
        if(PDGc[i].PDGm==PDGid&&PDGc[i].BRm*BRa>BR)
        {
            jest=1;
            for(short int j=0;j<PDGc[i].NDAm;j++){rozpad_ciagi(PDGc[i].NDA[j],x,y,PDGc[i].BRm*BRa);}
            y++;for(short int j=0;j<x;j++){PDGciag[j][y]=PDGciag[j][y-1];}
        }
    }
    if(jest!=1)
    {
        PDGciag[x][y]=PDGid;x++;
    }
}*/

/**int rciag[20][100];
int numx,numy;
int rnumx[100],rnumy[100];
double rprawd[100];*/

int ile_rozpadow(int PDGid)
{
    int wynik=0;///na ile wersji mo¿e siê rozpaœæ
    char PDGchar[10];
    int PDGpor;
    double BRp;
    int nrciagx=0,nrciagy=0;
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
        //short int k=0;nrc++;
        //do{BRchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);///pobranie BR
        //BRp=char_to_br();///zamiana BR na int
        //if(BRp>BR&&(PDGid==PDGpor||PDGpor==PDGid*(-1))){wynik++;}
        if(PDGid==PDGpor||PDGpor==PDGid*(-1))
        {
            bool znak=0;
            short int k=0;nrc++;
            do{BRchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);///pobranie BR
            BRp=char_to_br();///zamiana BR na int
            if(BRp>=BR&&BRp>0.0){wynik++;}
        }
    }
    wczyt2.close();wczyt2.clear();
    return wynik;
}

/**void rozpad_c(int PDGid, int prawd, int numerx, int numery)
{
    ;
}

void rozpad_c_poczatek(int PDGid)
{
    numx=0;numy=0;
    for(short int i=0;i<100;i++){rnumx[i]=-2;rnumy[i]=-2;rprawd[i]=0.0;}
    double prawd=1.0;
    bool jest;
    int ileroz=ile_rozpadow(PDGid);
    rciag[numx][numy]=PDGid;rnumx[numy]=-1;rnumy[numy]=-1;rprawd[numy]=1.0;numx++;numy++;
    for(int x=0;x<10;x++)
    {
        for(int y=0;y<100;y++)
        {
            if(rciag[x][y]!=0)
            {
                jest=0;
                for(short int i=0;i<lPDGmax;i++){if(PDGrozpadane[i]==rciag[x][y]){jest=1;break;}}
                if(jest==1){rozpad_c(rciag[x][y],prawd,x,y);}
            }
        }
    }
}*/

void rozpad_ciagi(int PDGid, int num)
{
    short int numer=0;
    char PDGchar[10];
    wczyt2.open("decays.txt");
        short int nrcw=0;
        int PDGpor,NDA,PDGnowe;
        double BRp;
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
                BRp=char_to_br();///zamiana BR na int
                if(BRp>=BR&&BRp>0.0)
                {
                    if(numer==num)
                    {
                        nrc++;
                        NDA=char_to_int(linia[nrc]);nrc+=2;///pobranie NDA
                        if(NDA>0)
                        {
                            int PDGnowe;
                            for(short int i=0;i<NDA;i++)
                            {
                                PDGnowe=0;
                                k=0;do{PDGchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);nrc++;
                                for(short int j=0;j<k;j++)
                                {
                                    liczba=char_to_int(PDGchar[j]);
                                    if(liczba>=0){PDGnowe*=10;PDGnowe+=liczba;}
                                    else{znak=1;}
                                }
                                if(znak==1){PDGnowe*=(-1);}
                                if(PDGid<0){PDGnowe*=(-1);}
                                rciag[nrx][nry]=PDGnowe;nrx++;
                            }
                        }
                        rprawd[nry]=BRp;
                        break;
                        //cout<<"nr"<<nrcw<<" NDA: "<<NDA[nrcw]<<", BR: "<<BRp[nrcw];
                        //for(short int i=0;i<NDA[nrcw];i++){cout<<", P("<<i<<"): "<<PDGnowe[nrcw][i];}
                        //cout<<endl;
                    }
                    else{numer++;}
                }
            }
        }
        wczyt2.close();wczyt2.clear();
}

void rozpad_ciagi_poczatek()
{
    int ileroz;nrx=0;nry=0;
    for(short int y=0;y<rymax;y++)
    {
        for(short int x=0;x<rxmax;x++)
        {
            rciag[x][y]=0;rir[x][y]=-1;
        }
        rprawd[y]=0.0;
    }
    for(short int i=0;i<lPDGmax;i++)
    {
        if(PDGrozpadane[i]!=0)
        {
            ileroz=ile_rozpadow(PDGrozpadane[i]);
            for(short int j=0;j<ileroz;j++)
            {
                rciag[nrx][nry]=PDGrozpadane[i];nrx++;
                rozpad_ciagi(PDGrozpadane[i],j);
                for(short int i=0;i<nrx;i++){if(rir[i][nry]==-1&&rciag[i][nry]!=0){rir[i][nry]=ile_rozpadow(rciag[i][nry]);}}
                nry++;nrx=0;
            }
        }
    }
    for(short int i=0;i<rymax;i++){if(rciag[0][i]==0){ymax=i;break;}}
}

int tworzenie_ciagow(int PDGid, double prawdop, int nrxx, int nryy, int nx=0, int ny=0)
{
    int kopia[kmax],knr=0,numer;
    int nrxs=nrxx;
    for(short int i=0;i<rir[nx][ny];i++)///numer wersji
    {
        numer=0;kopia[knr]=nryy;nrxx=nrxs;
        for(short int y=0;y<ymax;y++)
        {
            if(numer<=i)
            {
                if(rciag[0][y]==PDGid||rciag[0][y]==PDGid*(-1))
                {
                    if(numer==i&&prawdop*rprawd[y]>0.0)
                    {
                        if(prawdop*rprawd[y]>=BR)
                        {
                            ciagBR[nryy]=prawdop*rprawd[y];
                            ///cout<<nrxx<<"/"<<nryy<<": "<<y<<"("<<PDGid<<")"<<endl;///sprawdzenie
                            ciagPDG[nrxx][nryy]=y;nrxx++;
                            for(short int x=1;x<rxmax;x++)
                            {
                                if(rir[x][y]==-1){break;}
                                else
                                {
                                    if(PDGid>0||jest_neutralny(PDGid)==1)///rozpad zwyk³y
                                    {
                                        if(rir[x][y]==0){ciagPDG[nrxx][nryy]=2000+x;nrxx++;}
                                        else
                                        {
                                            nrxx=tworzenie_ciagow(rciag[x][y],prawdop*rprawd[y],nrxx,nryy,x,y);
                                        }
                                    }
                                    else///rozk³ad odwrotny
                                    {
                                        if(rir[x][y]==0){ciagPDG[nrxx][nryy]=(2000+x)*(-1);nrxx++;}
                                        else
                                        {
                                            nrxx=tworzenie_ciagow(rciag[x][y]*(-1),prawdop*rprawd[y],nrxx,nryy,x,y);
                                        }
                                    }
                                }
                            }
                        }
                        else///bez rozpadu
                        {
                            ciagBR[nryy]=prawdop;
                            for(short int x=0;x<rxmax;x++)
                            {
                                if(rir[x][y]==-1){break;}
                                else
                                {
                                    if(PDGid>0||jest_neutralny(PDGid)==1)///rozpad zwyk³y
                                    {
                                        if(rir[x][y]==0){ciagPDG[nrxx][nryy]=2000+x;nrxx++;}
                                        else
                                        {
                                            ciagPDG[nrxx][nryy]=y;nrxx++;
                                            ciagPDG[nrxx][nryy]=2000;nrxx++;
                                        }
                                    }
                                    else///rozk³ad odwrotny
                                    {
                                        if(rir[x][y]==0){ciagPDG[nrxx][nryy]=(2000+x)*(-1);nrxx++;}
                                        else
                                        {
                                            ciagPDG[nrxx][nryy]=y;nrxx++;
                                            ciagPDG[nrxx][nryy]=-2000;nrxx++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    numer++;
                }
            }
        }
        knr++;
        do{nryy++;}while(ciagPDG[0][nryy]!=-1);
    }
    for(short int i=1;i<knr;i++)///kopiowanie
    {
        for(short int x=0;x<nrxs;x++){ciagPDG[x][kopia[i]]=ciagPDG[x][kopia[0]];}
    }
    return nrxx;
}
void tworzenie_ciagow_poczatek(int PDGid)
{
    int nrxx,nryy,numer=0;
    double prawdop=1.0;
    for(short int x=0;x<cxmax;x++)
    {
        for(short int y=0;y<cymax;y++){ciagPDG[x][y]=(-1);}
    }
    nrxx=0;nryy=0;
    for(short int y=0;y<ymax;y++)
    {
        if((rciag[0][y]==PDGid||rciag[0][y]==PDGid*(-1))&&prawdop*rprawd[y]>=BR&&prawdop*rprawd[y]>0.0)///pocz¹tek tworzenia ci¹gu koñcowego
        {
            ///cout<<"x/y"<<y<<"/"<<nry<<":";
			ciagPDG[nrxx][nryy]=y;ciagBR[nryy]=prawdop*rprawd[y];nrxx++;
            for(short int x=1;x<rxmax;x++)
            {
                if(rir[x][y]==-1){break;}
                else
                {
                    if(rir[x][y]==0){ciagPDG[nrxx][nryy]=2000+x;nrxx++;if(nrxx>cxmax){nrxx=cxmax-1;}}
                    else
                    {
                        nrxx=tworzenie_ciagow(rciag[x][y],prawdop*rprawd[y],nrxx,nryy,x,y);
                    }
                }
            }
            do{nryy++;}while(nryy<400&&ciagPDG[0][nryy]!=-1);nrxx=0;
        }
    }
    for(short int i=0;i<cymax;i++){if(ciagPDG[0][i]==-1){ymax=i;break;}}
}

void ciagi_sortowanie()
{
    int zast;
    double zastBR;
    for(short int i=0;i<ymax;i++)
    {
        for(short int j=ymax-1;j>i;j--)
        {
            if(ciagBR[j]>ciagBR[j-1])///zamiana miejsc
            {
                for(short int x=0;x<cxmax;x++){zast=ciagPDG[x][j];ciagPDG[x][j]=ciagPDG[x][j-1];ciagPDG[x][j-1]=zast;}
                zastBR=ciagBR[j];ciagBR[j]=ciagBR[j-1];ciagBR[j-1]=zastBR;
            }
        }
    }
}

void ciagi_odczyt()
{
    int numer;
    int xx=0,yy=0;
    for(short int y=0;y<ymax;y++)
    {
        if(ciagPDG[0][y]!=-1)
        {
            numer=0;
            cout<<"{"<<rciag[0][ciagPDG[0][y]]<<",{";
            if(wyjscie==1){zapis<<"{"<<rciag[0][ciagPDG[0][y]]<<",{";}
            for(short int x=0;x<20;x++)
            {
                if(ciagPDG[x][y]>=2000){xx=ciagPDG[x][y]-2000;cout<<rciag[xx][yy];if(wyjscie==1){zapis<<rciag[xx][yy];}if(x<19&&ciagPDG[x+1][y]!=-1){cout<<",";if(wyjscie==1){zapis<<",";}}}
                else
                {
                    if(ciagPDG[x][y]<=-2000){xx=(ciagPDG[x][y]+2000)*(-1);cout<<rciag[xx][yy]*(-1);if(wyjscie==1){zapis<<rciag[xx][yy]*(-1);}if(x<19&&ciagPDG[x+1][y]!=-1){cout<<",";if(wyjscie==1){zapis<<",";}}}
                    else{if(ciagPDG[x][y]>=0){yy=ciagPDG[x][y];}}
                }
            }
            cout<<"},"<<ciagBR[y]<<"}\n";
            if(wyjscie==1){zapis<<"},"<<ciagBR[y]<<"}\n";}
        }
    }
}

/**void rozpad_poczatek_ciagi()
{
    short int x=0,y=0;
    bool jest_rozp=0;BRact=1.0;
    if(PDG!=0)
    {
        PDGciag[x][y]=PDG;x++;
        for(short int i=0;i<lPDGmax;i++)
        {
            if(PDGrozpadane[i]==PDGciag[x][y])
            {
                jest_rozp=1;
                for(short int j=0;j<lPDGmax;j++)
                {
                    if(PDGc[j].PDGm==PDG){rozpad_ciagi(PDGc[j].PDGm,x,y,BRact);y++;}
                }
                break;
            }
        }
    }
    else
    {
        for(short int i=0;i<lPDGmax;i++)
        {
            if(PDGrozpadane[i]!=0)
            {
                for(short int j=0;j<lPDGmax;j++)
                {
                    if(PDGc[j].PDGm==PDGrozpadane[i]){PDGciag[x][y]=PDGrozpadane[i];x++;rozpad_ciagi(PDGc[j].PDGm,x,y,BRact);y++;}
                }
                x=0;
            }
        }
    }
}

void rozpad_poczatek()
{
    int numer=0;
    for(short int nr=0;nr<ilerozp;nr++)
    {
        short int xx=1;
        int ile_rozpadow=0;///na ile wersji mo¿e siê rozpaœæ
        char PDGchar[10];
        int PDGpor;
        int BRp;
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
            short int k=0;nrc++;
            do{BRchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);///pobranie BR
            BRp=char_to_br();///zamiana BR na int
            if(BRp>BR)
            {
                PDGc[numer].PDGm=PDGpor;
                PDGc[numer].BRm=BRp;
                nrc++;
                PDGc[numer].NDAm=char_to_int(linia[nrc]);nrc+=2;///pobranie NDA
                for(short int i=0;i<PDGc[numer].NDAm;i++)
                {
                    PDGc[numer].NDA[i]=0;
                    k=0;do{PDGchar[k]=linia[nrc];nrc++;k++;}while(linia[nrc]!='#'&&k<15&&nrc<100);nrc++;
                    for(short int j=0;j<k;j++)
                    {
                        liczba=char_to_int(PDGchar[j]);
                        if(liczba>=0){PDGc[numer].NDA[i]*=10;PDGc[numer].NDA[i]+=liczba;}
                        //else{znak=1;}
                    }
                    //if(znak==1){PDGc[numer].NDA[i]*=(-1);}
                    //if(PDGid<0){PDGc[numer].NDA[i]*=(-1);}///edit
                }
                numer++;
            }
        }
        wczyt2.close();wczyt2.clear();
    }
}*/

/*void rozpad(int PDGid, int x, int y)
{
    short int nr;
    for(short int i=0;i<ciagile;i++)
    {
        if(PDGciag[i][0]==PDGid){nr=i;break;}
    }
    if(nr<ilerozp)
    {
        ;
    }
}*/

void lPDGrozpad()
{
    int PDGpob;int numpom;
    for(short int i=0;i<lPDGmax;i++){PDGrozpadane[i]=0;}
    wczyt.open("CMN.txt");
    while(!wczyt.eof())///odwo³ujemy siê do pliku z numerami PDG
    {
		///pobranie numeru PDG
        PDGpob=0;
        wczyt.getline(linia,100);
        short int i=0,liczba;
        do
        {
            liczba=char_to_int(linia[i]);
            PDGpob*=10;PDGpob+=liczba;
            i++;
        }while(linia[i]!='#'&&linia[i]!='\0'&&i<100);
        if(PDGpob>0)
        {
            PDGrozpadane[ilerozp]=PDGpob;
            ilerozp++;
        }
    }
    wczyt.close();wczyt.clear();
}

void lista_neutral()
{
    int PDGpob;int numpom;
    for(short int i=0;i<nmax;i++){neutral[i]=0;}
    wczyt.open("neutral.txt");
    if(!wczyt){cout<<"Nie znaleziono pliku neutral.txt!\n";}
    else
    {
        int xx=0;
        while(!wczyt.eof())///odwo³ujemy siê do pliku z numerami PDG
        {
            ///pobranie numeru PDG
            PDGpob=0;
            wczyt.getline(linia,100);
            short int i=0,liczba;
            do
            {
                liczba=char_to_int(linia[i]);
                PDGpob*=10;PDGpob+=liczba;
                i++;
            }while(linia[i]!='#'&&linia[i]!='\0'&&i<100);
            if(PDGpob>0)
            {
                neutral[xx]=PDGpob;
                xx++;
            }
        }
    }
    wczyt.close();wczyt.clear();
}

/**void wyniki()
{
    for(short int i=0;i<ciagile;i++)
    {
        if(PDGciag[0][i]!=0&&BRciag[i]>BR)
        {
            cout<<"{"<<PDGciag[0][i];
            if(PDGciag[1][i]!=0)
            {
                cout<<",{";
                for(short int j=1;j<ciagmax;j++){if(PDGciag[j][i]!=0){cout<<PDGciag[j][i]<<",";}}
                cout<<"}";
            }
            cout<<","<<BRciag[i]<<"}\n";
        }
    }
}*/

int main(int argc, char** argv)///pobieranie parametrów
{
	lista_neutral();
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
	//for(int i=0;i<ciagmax;i++){BRciag[i]=0.0;for(short int j=0;j<ciagile;j++){PDGciag[i][j]=0;}}
	lPDGrozpad();///przygotowanie listy rozpad³ych PDG
	rozpad_ciagi_poczatek();///przygotowanie tabelki rozpadów
	/**for(short int y=0;y<100;y++)///sprawdzanie
    {
        if(rciag[0][y]!=0)
        {
        	cout<<y<<" - ";
			for(short int x=0;x<10;x++)
        	{
            	if(rciag[x][y]!=0){cout<<rciag[x][y]<<"("<<rir[x][y]<<") ; ";}
        	}
        	cout<<"BR="<<rprawd[y]<<endl;
        }
    }*/
    if(wszystkie!=1)
    {
        tworzenie_ciagow_poczatek(PDG);ciagi_sortowanie();
        /**for(short int y=0;y<100;y++)///sprawdzanie
        {
            if(ciagPDG[0][y]!=-1)
            {
                cout<<y<<" - ";
                for(short int x=0;x<20;x++)
                {
                    if(ciagPDG[x][y]!=-1){cout<<ciagPDG[x][y]<<", ";}
                }
                cout<<endl;
            }
        }*/
        ciagi_odczyt();
    }
    else
    {
        for(short int i=0;i<lPDGmax;i++)
        {
            if(PDGrozpadane[i]!=0)
            {
                tworzenie_ciagow_poczatek(PDGrozpadane[i]);ciagi_sortowanie();
                ciagi_odczyt();
            }
        }
    }
    /*if(wszystkie!=1){cout<<"{";rozpad(PDG,1,0);cout<<"}";}
    else
    {
        for(short int i=0;i<lPDGmax;i++)
        {
			PDG=PDGrozpadane[i];
            ///rozpad
            if(PDG>0)
            {
                //cout<<i<<"/"<<lPDGmax<<" ("<<PDG<<"):\n";
				rozpad(PDG,1,0);
            }
        }
    }*/
    //rozpad_poczatek();
    //rozpad_poczatek_ciagi();
    //wyniki();
    zapis.close();
    wczyt2.close();
    fremover();///usuwanie plików pomocniczych
	/**
	argc - liczba argumentów
	argv[0] - nazwa programu
	argv[1] - parametr 1...
	*/
	return 0;
}
