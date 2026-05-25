#include<iostream>

using namespace std;

class TimeConverter{
		
	public:
		
		void getFormat(int s)
		{
			int hr, min, sec;
			
			hr = s / 3600;
			min = (s / 60) - (hr * 60);
			sec = s - (min * 60) - (hr * 3600);
				
			cout << hr << ":" << min << ":" << sec;
		}
		
		void getSeconds(int h, int m, int s)
		{
			int hr, min, sec;
			
			hr = h * 3600;
			min = m * 60;
			sec = s;
				
			cout << hr + min + sec;
		}
		
		
};


int main()
{
	int sec1, sec2, hr, min;
	
	TimeConverter t1;
	
	
	cout << endl <<"---------- Time-Converter Project ----------" << endl;
	cout << endl << "---------- Seconds To HH:MM:SS convert ----------";
	cout<< endl << endl << "Enter Total Seconds :- ";
	cin >> sec1;
	
	cout << "HH:MM:SS -> ";
	t1.getFormat(sec1);
	
	cout << endl << endl << "---------- HH:MM:SS to Seconds convert ----------";
	cout << endl << endl <<"Enter Hours :- ";
	cin >> hr;
	cout <<"Enter Minutes :- ";
	cin >> min;
	cout << "Enter Seconds :- ";
	cin >> sec2;
	
	cout << endl << endl << "Total seconds :- ";
	t1.getSeconds(hr,min,sec2);
	
	return 0;
}