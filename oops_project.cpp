#include <iostream>
#include <string>
#include <cstdio>
#include <exception>
using namespace std;
class player {
private:
    string name;
    int j_id;
    string type;
    int runs;
    int wickets;
    static int tp; 

public:
     player() {
        this->name = "";
        this->j_id = 0;
        this->type = "";
        this->runs = 0;
        this->wickets = 0;
        tp++;
    }    

    player(string n, int id, string t) {
        this->name = n;
        this->j_id = id;
        this->type = t;
        this->runs = 0;
        this->wickets = 0;
        tp++;
    }
    ~player(){

    }
    void setdata(string name, int j_id, string type) {
        this->name = name;
        this->j_id = j_id;
        this->type = type;
    }

    void setdata(string name, int j_id) {
        this->name = name;
        this->j_id = j_id;
        type = "bowler"; 
    }

    void getdata(int runs, int wickets) {
        this->runs = runs;
        this->wickets = wickets;
    }

    int getruns(){
         return runs; 
    }
    int getwickets(){ 
        return wickets; 
    }
    string gettype(){
         return type; 
        }
    string getname(){
         return name; 
        }
    int getID(){
         return j_id; 
        }

    static int showtotal(){
         return tp; 
    }

    bool operator>(player &p) {  
        if (this->runs > p.runs) {
            return true;
        }
        if (this->runs == p.runs && this->wickets > p.wickets) {
            return true;
        }
        return false;
    }

    friend void showplayers(player p);
};
int player::tp = 0;
void showplayers(player p) {
    cout << "Name: " << p.name << " ID: " << p.j_id  << " Type: " << p.type << " Runs: " << p.runs  << " Wickets: " << p.wickets << "\n";
}

int main() {
    int n;
    cout << "Enter number of players: "; 
    cin >> n;
    player *p = new player[n];
    int total_runs = 0;
    int total_wickets = 0;

    for (int i = 0; i < n; i++) {
        try {
            string name, type;
            int j_id;

            cout << "Enter details for Player " <<"\n";
            cout << "Name: ";
            cin >> name;

            while (true) {
                cout << "ID: ";
                cin >> j_id;
                if (cin.fail()) {
                    cout << "Invalid ID\n";
                    cin.clear();
                    cin.ignore(10, '\n');
                } else break;
            }

            while (true) {
                cout << "Type (batsman/bowler/allrounder): ";
                cin >> type;
                if (type == "batsman" || type == "bowler" || type == "allrounder") {
                    break;
                }
                else {
                    cout << "Enter batsman, bowler, or allrounder.\n";
                }
            }

            if (type == "bowler") p[i].setdata(name, j_id);
            else p[i].setdata(name, j_id, type);

            int runs = 0, wickets = 0;
            cout << "Enter runs scored:";
            cin >> runs;
            string wicketInput;
            while(true){
                cout << "Wicket? (Y/N): ";
                cin >> wicketInput;
                if (wicketInput == "Y" || wicketInput == "y") {
                    wickets = 1;
                    total_wickets++;
                    break;
                } else if (wicketInput == "N" || wicketInput == "n") {
                    wickets = 0;
                    break;
                } else cout << "wrong input\n";
            }

            p[i].getdata(runs, wickets);
            total_runs += runs;

        } catch (exception &e) {
            cout << "Invalid input!\n";
        }
    }

    cout << "player Details"<<endl;
    for (int i = 0; i < n; i++) {
        showplayers(p[i]);
    }

    player best = p[0];
    for (int i = 1; i < n; i++)
        if (p[i] > best) {
            best = p[i];
        } 

    cout << "Best Player\n ";
    showplayers(best);

    cout << "total players: " << player::showtotal() << "\n";
    cout << "total Runs: " << total_runs << "\n";
    cout << "total Wickets: " << total_wickets << "\n";

    
    delete[] p;

    return 0;
}
