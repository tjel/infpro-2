# include <iostream>

using namespace std;

int main( ) 
{ 
    float PI = 3.1415;                // variables can be initialized during declaration 
    float rad; 
    cout<<"Enter the radius: "; 
    cin>>rad;
    if(rad>0){
    	cout<< "Area of the circle is "<< PI * rad * rad;
    }else{cout<<"Radius must be > 0!"<<endl;} 
   
    return 0;
}  
