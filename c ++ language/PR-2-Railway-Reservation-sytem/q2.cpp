#include<iostream>

using namespace std;

class Train{
	
	
	private:
		
		int trainNo;
		string traiName, trainSource, trainDest, trainTime;
		
	public:
		
		bool found = false;
		
		static int getTraincount;
		
		Train(){};
		
		Train(int a, string name, string src, string dest, string tim)
		{
			trainNo = a;
			traiName = name;
			trainSource = src;
			trainDest = dest;
			trainTime = tim;
		}
		
		void getTrdetails(Train &obj)
		{
			cout << "Train No :- " << obj.trainNo << endl;
			cout << "Train Name :- " << obj.traiName << endl;
			cout << "Train Source :- "<< obj.trainSource << endl;
			cout << "Train Destination :- " << obj.trainDest << endl;
			cout << "Train time :- "<< obj.trainTime << endl;
		}
		
		void serTrain(Train &obj,int a)
		{
			if(obj.trainNo == a)
			{
				obj.getTrdetails(obj);
				found = true;
			}
			
			if(!found)
			{
				cout <<"Train number with " << a << " is not found...!!" << endl;
			}
		}

};

int Train::getTraincount=0;

class Railwaysystem{
	
	private:
		
		int totalTrains = 0;
	
	public:
		
		void addTrain(Train &obj)
		{
			int trno;
			string name, src, dest, time;
			
			cout << "Enter Train No :- ";
			cin >> trno;
			cout << "Enter Train Name :- ";
			cin.ignore();
			getline(cin,name);
			cout << "Enter Train's Source :- ";
			getline(cin,src);
			cout << "Enter Train's Destination :- ";
			getline(cin,dest);
			cout << "Enter Train's Time (in HH:MM) :- ";
			cin >> time;
			
			obj = Train(trno,name,src,dest,time);
			totalTrains++;
		}
		
		void displayTrains(Train &obj)
		{
			obj.getTrdetails(obj);
		}
		
		void searchTrain(Train &obj, int a)
		{
			
			obj.serTrain(obj,a);
			
		}
		
	
};


int main()
{
	
	int n, i, temp = 1;
	int srnum;
	int getTraincount = 0;
	
	cout << "Welcome, To Railway Reservation System...!!" << endl;
	
	cout << endl << "Enter number of trains you want to add :- ";
	cin >> n;
	
	Train tr[n];
	Railwaysystem rst;
	int choice;
	
	do{
	
	
		cout << endl << "Press 1 for add train data.";
		cout << endl << "Press 2 for display all train records.";
		cout << endl << "Press 3 for search train.";
		cout << endl << "Press 0 for exit.";
		cout << endl << endl;
	
	
		cout << "Enter Your Choice :- ";
		cin >> choice;
	

	
		switch (choice)
		{
			case 1:
				
				cout << endl << endl << "Please Enter details...!!" << endl << endl;
				for(i=0;i<n;i++)
				{
					rst.addTrain(tr[i]);
					cout << endl;
					getTraincount++;
				}
				break;
			
			case 2:
				
				if(getTraincount > 0)
				{
					cout << endl << endl << "Here is the details...!!" << endl << endl;
					for(i=0;i<n;i++)
					{
						cout << "Train " << temp << "'s details" << endl;
						rst.displayTrains(tr[i]);
						cout << endl << endl;
						temp++;	
					}	
				}
				else
				{
					cout << endl << "Record Is empty, please first enter records...!!";
				}
				
				break;
				
			case 3:
				
				cout << endl << "Enter Train Number :- ";
				cin >> srnum;
			
				for(i=0;i<n;i++)
				{
					rst.searchTrain(tr[i],srnum);
				}
				break;
			
			case 0:
				
				cout << endl << "You Are Exited...." << endl << "Thanks for visiting...!!";
				break;
				
			default:
				cout << endl <<"Please select valid choice...!!" << endl;
				break;
		}
	
	}while(choice != 0);
	
	return 0;
}