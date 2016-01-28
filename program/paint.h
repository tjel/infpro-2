#include <iostream>	
#include <fstream>
#include <cstdlib>
#include <cairo.h>
#include <stack>
using namespace std;

	double pw = 500;
	double ph = 500;
	double poziom = 30;
	cairo_surface_t *surface=cairo_image_surface_create (CAIRO_FORMAT_ARGB32, pw, ph);
  cairo_t *cr=cairo_create (surface);
	string tabh[100];
	string tabl[100];
	stack<string>stos;
	stack<string>wysokosc;
	string produkt;

void paint(string pdg){

	// zmienne
	string line;
	fstream file;
	file.open("decays.txt",ios::in);
	void rozpad(string nr,double left,double right,int level,int x,int y);
	int level = 1;
	for(int i=0;i<100;i++){
		tabh[i]="*";
		tabl[i]="*";	
	}
	
	// tworzenie pliku + rysowanie czastki
	cairo_rectangle (cr, 0, 0, 500, 500);
	cairo_set_source_rgba (cr, 1, 1, 1, 1);
	cairo_fill (cr);
  cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 10.0);
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_set_line_width (cr, 1);
  cairo_move_to (cr, pw/2-(3.5*pdg.length()), poziom);
  cairo_show_text (cr, pdg.c_str());

	// rysowanie kreski glownej
	cairo_move_to (cr, pw/2, poziom+2);
	poziom*=level;
	cairo_line_to (cr, pw/2, poziom+32);
	cairo_set_line_width (cr, 1);
	cairo_stroke (cr);

	// wyszukanie czastki
	if(file.good()){
		getline(file,line);
		while(!file.eof()){	
        	if( (line.length()>=pdg.length()+1) && (line.substr(0,pdg.length()+1)==pdg+"#") ){
						tabh[0]==pdg;
						rozpad(pdg,0,500,level,pw/2,poziom+2);
						break;
					}
			getline(file,line);
		}

	// zamykanie pliku
	cairo_destroy (cr);
  cairo_surface_write_to_png (surface, "rozpad.png");
  cairo_surface_destroy (surface);	

	}else{cout<<"Brak dostepu do pliku decays.txt"<<endl;}
	file.close();
}

void rozpad(string nr,double left,double right,int level,int x,int y){

	// zmienne
	fstream file;
	file.open("decays.txt",ios::in);
	string line;
	int i,j,k,l;
	int hasz=0;
	int	r;
	int kom,tkom; 
	int mar = 2;
	string tab[100];
	for(int i=0;i<100;i++){
		tab[i]="*";	
	}


	// wyszukiwanie ilosci produktow rozpadu	
	if(file.good()){
		getline(file,line);
		while(!file.eof()){	
    	if( (line.length()>=nr.length()+1) && (line.substr(0,nr.length()+1)==nr+"#") ){
				hasz=0;
				for(i=0;i<line.length();i++){
					if(line[i]=='#'){hasz+=1;}
					if(hasz==2){
						r=atoi((line.substr(i+1,1)).c_str()); // na ile sie dzieli
						i++;
						break;
					}
				}
				i+=2;			
				// zapisywanie produktow rozpadu do tablicy			
				for(j=0;j<r;j++){
					tabl[j]="";
					while(line[i]!='#'){
						tabl[j]+=line[i]; // tablica z produktami
						i++;
					}
					i++;	
				}
				
				for(j=r-1;j>=0;j--){
					stos.push(tabl[j]);		
				}			

				//************RYSOWANIE ROZPADU***********
				int c=0;
				// rysowanie poziomej
				kom=(right-left)/(r+1);
				tkom = kom;
				level++;
				cairo_move_to (cr, kom, poziom*level+mar);
				cairo_line_to (cr, tkom*r, poziom*level+mar);
				
				cairo_stroke (cr);
				for(l=0;l<r;l++){	
					//left=(l+1)*tkom-0.5*tkom;
					//right=left+tkom;
					cairo_move_to(cr,kom,poziom*level+mar);
					level++;
					cairo_line_to (cr,kom,poziom*level+mar);
					cairo_stroke (cr);
					cairo_move_to (cr, kom+mar,poziom*level-mar);
					produkt=stos.top();
					stos.pop();				
					cairo_show_text (cr, produkt.c_str());
					level--;
					rozpad(produkt.c_str(),kom,kom*r,level,left,poziom);
					//rozpad(produkt.c_str(),left,kom*r,level,left,poziom);
					kom+=tkom;
				}
				//**************************************** 
			break;				
			}
			getline(file,line);
		}
	}else{cout<<"Brak dostepu do pliku decays.txt"<<endl;}
	file.close();
}
