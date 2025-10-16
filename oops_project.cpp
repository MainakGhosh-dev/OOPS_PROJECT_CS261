#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <exception>
#include <stdexcept>
using namespace std;

class BaseStats {
public:
    virtual void showStats() {
        cout << "Cricket game.\n";
    }


    virtual void winner(string& teamName, int margin) {
        cout << "Team " << teamName << " wins by " << margin << " runs/wickets!\n";
    }

    virtual void winner() {
        cout << "The match ended in a draw!\n";
    }

    virtual ~BaseStats() {

    }
};


class Player : public BaseStats {
public:
    string name;
    int r, b, four, six, w, ov, con;

    Player(string n = "") {
        this->name = n;
        this->r = this->b = this->four = this->six = this->w = this->ov = this->con = 0;
    }
    void resetBowlingStats() {
        this->w = 0;
        this->ov = 0;
        this->con = 0;
        this->b = 0;  
    }
    virtual int strikeRate() {
        if (this->b == 0) return 0;
        return (this->r * 100.0) / this->b;
    }

    virtual int economy() {
        if (this->ov == 0) return 0;
        return this->con / (int)this->ov;
    }

    Player& operator++() {
        this->b++;
        return *this;
    }

    void addBall() { 
        this->b++; 
    }
    void addConceded(int runs) { 
        this->con += runs; 
    }

    virtual void showStats() {
        cout << "Player Stats: " << name << "  Runs: " << r << "  Balls: " << b << endl;
    }

    virtual ~Player() {}
};


class Team : public Player {
public:
    Player* pl;
    int cnt;
    int tr, wkt, ob, bb;

    Team(string n = "", int pCnt = 0) : Player(n) {
        this->cnt = pCnt;
        this->tr = this->wkt = this->ob = this->bb = 0;
        if (pCnt > 0) {
            try {
                this->pl = new Player[pCnt];
            } catch (bad_alloc& e) {
                cout << " allocation fail" << endl;
                throw runtime_error("allocate fail");
            }
        } else {
            this->pl = nullptr;
        }
    }

    void addPlayer(string& pname, int idx) {
        if (idx < 0 || idx >= this->cnt) {
            throw out_of_range("Invalid player index");
        }
        this->pl[idx] = Player(pname);
    }
    
    void resetBowling() {
        for (int i = 0; i < this->cnt; ++i) {
            this->pl[i].resetBowlingStats();
        }
    }
    
    void showb() {
        cout << "\nBatting " << this->name << "\n";
        cout << "Name  Runs  Balls  4s  6s  SR\n";
        for (int i = 0; i < this->cnt; ++i) {
            cout << this->pl[i].name << "  " << this->pl[i].r << "  " << this->pl[i].b << "  " << this->pl[i].four << "  " << this->pl[i].six << "  " << this->pl[i].strikeRate() << "\n";
        }
    }

    void showbo() {
        cout << "\nBowling " << this->name << "\n";
        cout << "Name  Overs  Runs  Wkts  Eco\n";
        for (int i = 0; i < this->cnt; ++i) {
            if (this->pl[i].b > 0) {
                int completedOvers = this->pl[i].b / 6;
                int remainingBalls = this->pl[i].b % 6;
                cout << this->pl[i].name << "  " << completedOvers;
                if (remainingBalls > 0) {
                    cout << "." << remainingBalls;
                }
                cout << "  " << this->pl[i].con 
                     << "  " << this->pl[i].w << "  ";
                if (this->pl[i].b > 0) {
                    cout << (this->pl[i].con * 6.0) / this->pl[i].b;
                } else {
                    cout << 0;
                }
                cout << "\n";
            }
        }
    }

    virtual void showStats() {
        cout << "Team Stats: " << name << "  Total Runs: " << tr << "  Wickets: " << wkt << endl;
    }

    virtual void winner(string& teamName) {
        cout << "Team " << teamName << " wins the match!\n";
    }
    void winner(string& teamName, int margin) override {
    cout << " " << teamName << " wins magnificently by " << margin << " runs/wickets!\n";
}

    ~Team() {
        delete[] pl;
    }

    friend void manOfTheMatch(Team& t1, Team& t2);
};


class Match : public Team {
public:
    Team tA, tB;
    int ov;
    int str, ns, bow;

    Match(string a, string b, int o, int pCnt)
        : tA(a, pCnt), tB(b, pCnt) {
        if (o <= 0) {
            throw invalid_argument("Overs must be greater than 0");
        }
        if (pCnt <= 1) {
            throw invalid_argument("Players must be greater than 1");
        }
        ov = o;
        str = ns = bow = -1;
    }

    void sti(Team &bat, Team &bowl, bool is2nd = false, int tgt = 0, int n = 0);
    void upsc(Team &bat, Team &bowl, int evt);
    bool checkEnd(Team &bat, Team &bowl, int tgt, int n);
};



void Match::sti(Team &bat, Team &bowl, bool is2nd, int tgt, int n) {
    try{
        cout << "\nstarting  " << bat.name << " batting\n";
    bat.tr = bat.wkt = bat.ob = bat.bb = 0;
    bowl.resetBowling(); 
    this->str = -1;
    while (this->str == -1) {
        cout << "Enter striker index (0 to " << bat.cnt - 1 << "): ";
        cin >> this->str;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10, '\n');
        }
        if (this->str < 0 || this->str >= bat.cnt) {
            cout << "Invalid index\n";
            this->str = -1;
        }
    }

    this->ns = -1;
    while (this->ns == -1) {
        cout << "enter non-striker index: ";
        cin >> this->ns;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10, '\n');
            throw invalid_argument("Invalid input");
        }
        if (this->ns < 0 || this->ns >= bat.cnt) {
            cout << "Invalid index\n";
            this->ns = -1;
        }
    }

    this->bow = -1;
    while (this->bow == -1) {
        cout << "Enter bowler index from " << bowl.name << " (0 to " << bowl.cnt - 1 << "): ";
        cin >> this->bow;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10, '\n');
            throw invalid_argument("Invalid input");
        }    
            if(this->bow<0 || this->bow >= bowl.cnt){
                
                cout<<"Invalid index";
                this->bow = -1;
            }
            
        
    }

    while (bat.ob < this->ov && bat.wkt < bat.cnt - 1) {
        int evt = -1; 
        while (true) {
            cout << "\nBall " << bat.ob << "." << (bat.bb % 6) + 1;
            cout << " - Enter event (0-6 runs, 7=wide, 9=wicket, 10=bye, 11=leg-bye): ";

            cin >> evt;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10, '\n');
                cout << "Invalid input.\n";
                continue; 
            }

            if (evt < 0 || evt > 12) {
                cout << "Invalid index.\n";
                continue; 
            }

            
            break;
        }

        

        if (evt > 12){
             throw out_of_range("Invalid input");
        }    

        upsc(bat, bowl, evt);

        if (is2nd && checkEnd(bat, bowl, tgt, n)) {
            return;
        }

        if (bat.ob >= this->ov || bat.wkt >= bat.cnt - 1) {
            break;
        }

        if (bat.bb % 6 == 0 && bat.bb > 0) {
            int opt = 0;
            while (opt != 3) {
                cout << "\n\n1. Show Batting\n2. Show Bowling\n3. Continue\nChoice: ";
                if (!(cin >> opt)) {
                    cin.clear(); 
                    cin.ignore(10, '\n');
                    cout << "invalid\n";
                    continue;
                }
                if (opt == 1) {
                    bat.showb();
                }
                else if (opt == 2) {
                    bowl.showbo();
                }
            }

            if (bat.ob < ov) {
                this->bow = -2;
                while (this->bow == -2) {
                    cout << "\nnew bowler index: ";
                    cin >> this->bow;
                    if (cin.fail()) {
                        cin.clear(); 
                        cin.ignore(10, '\n');
                        throw invalid_argument("Invalid bowler input");
                    }
                    if (this->bow < 0 || this->bow >= bowl.cnt) {
                        cout << "invalid index\n";
                        this->bow = -2;
                    }
                }
            }
        }
    }
    } catch (const invalid_argument&) {
        cout << "Invalid argument.\n";
    } catch (const out_of_range&) {
        cout << "Out of range error.\n";
    } catch (const exception&) {
        cout << "Error:exception occurred.\n";
    }
}


void Match::upsc(Team &bat, Team &bowl, int evt) {
    try {
        if (this->str < 0 || this->str >= bat.cnt) {
            throw out_of_range("Invalid striker index");
        }
        if (this->bow < 0 || this->bow >= bowl.cnt) {
            throw out_of_range("Invalid bowler index");
        }
        
        Player &batter = bat.pl[this->str];
        Player &bowler = bowl.pl[this->bow];
        int runs = 0;
        bool islegal=false;
       



        if(evt>=12){
            throw out_of_range("Invalid player index");
        }
        if (evt >= 0 && evt <= 6) {
            batter.r += evt;
            bat.tr += evt;
            bowler.addConceded(evt);
            if (evt == 4) batter.four++;
            if (evt == 6) batter.six++;
            runs = evt;
            ++batter;  
            ++bowler;  
            islegal=true;
        } else if (evt == 7) {
            bat.tr += 1;
            bowler.addConceded(1);
            islegal=false;
        } else if (evt == 9) {
            bat.wkt++;
            bowler.w++;
            ++batter;
            ++bowler;  
            islegal=true;
            if (bat.wkt < bat.cnt - 1) {
                do {
                    cout << "new batsman index: ";
                    cin >> this->str;
                    if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
                        cin.clear(); 
                        cin.ignore(10, '\n');
                        throw invalid_argument("Invalid batsman index");
                    }
                } while (this->str == -1);
            }
        } else if (evt == 10 || evt == 11) {
            cout << "Enter runs: ";
            cin >> runs;
            if (!cin) {
                cin.clear();
                cin.ignore(10, '\n');
                throw invalid_argument("Invalid runs input");
            }
            bat.tr += runs;
            ++batter;
            ++bowler;  
            islegal=true;
        } else {
            throw invalid_argument("Invalid event type");
        }

        if(islegal){
            bat.bb++;
            if(runs% 2 != 0){
                swap(this->str, this->ns);
            }
            if(bat.bb%6==0){
                bat.ob++;
                swap(this->str,this->ns);
            }
        }
    } catch (const invalid_argument&) {
        cout << "Invalid argument.\n";
    } catch (const out_of_range&) {
        cout << "Out of range error.\n";
    } catch (const exception&) {
        cout << "Error: an exception occurred.\n";
    }
}

bool Match::checkEnd(Team &bat, Team &bowl, int tgt, int n) {
    if (bat.tr > tgt) {
        cout << "\nResult\n" << bat.name << " won by " << (n - bat.wkt - 1) << " wickets\n";
        return true;
    }
    if (bat.ob >= this->ov || bat.wkt >= n - 1) {
        if (bat.tr <= tgt) {
            cout << "\nResult\n" << bowl.name << " won by " << (tgt - bat.tr) << " runs\n";
            return true;
        }
    }
    return false;
}



void manOfTheMatch(Team& t1, Team& t2) {
    Player bp;
    string tn;
    int maxRuns = 0, maxWickets = 0;

    for (int i = 0; i < t1.cnt; ++i) {
        if (t1.pl[i].r > maxRuns || t1.pl[i].w > maxWickets) {
            bp = t1.pl[i];
            tn = t1.name;
            maxRuns = t1.pl[i].r;
            maxWickets = t1.pl[i].w;
        }
    }

    for (int i = 0; i < t2.cnt; ++i) {
        if (t2.pl[i].r > maxRuns || t2.pl[i].w > maxWickets) {
            bp = t2.pl[i];
            tn = t2.name;
            maxRuns = t2.pl[i].r;
            maxWickets = t2.pl[i].w;
        }
    }

    cout << "\n Man of the Match: " << bp.name << " " << tn<< "\n";
    cout << "Runs: " << bp.r << " | Wickets: " << bp.w << endl;
}

int main() {
    try {
        string t1, t2;
        int ov, n, fb;
        cout << "Team A name: ";
        getline(cin, t1);
        cout << "Team B name: ";
        getline(cin, t2);
        
        cout << "Total overs: ";
        cin >> ov;
        if (!cin || ov <= 0) {
            cin.clear();
            cin.ignore(10, '\n');
            throw invalid_argument("Invalid overs input");
        }

        cout << "Players per team: ";
        cin >> n;
        if (!cin || n <= 1) {
            cin.clear();
            cin.ignore(10, '\n');
            throw invalid_argument("Invalid players count");
        }
        cin.ignore();

        Match match(t1, t2, ov, n);

        cout << "\nTeam A players:\n";
        for (int i = 0; i < n; i++) {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tA.addPlayer(name, i);
        }

        cout << "\nTeam B players:\n";
        for (int i = 0; i < n; i++) {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tB.addPlayer(name, i);
        }

        cout << "\nbat first (1=" << t1 << ", 2=" << t2 << "):";
        cin >> fb;
        if (fb != 1 && fb != 2) {
            throw invalid_argument("Invalid batting first choice");
        }

        if (fb == 1) {
            match.sti(match.tA, match.tB);
            match.tA.showb();
            match.tB.showbo();
            match.sti(match.tB, match.tA, true, match.tA.tr, n);
            match.tB.showb();
            match.tA.showbo();
        } else {
            match.sti(match.tB, match.tA);
            match.tB.showb();
            match.tA.showbo();
            match.sti(match.tA, match.tB, true, match.tB.tr, n);
            match.tA.showb();
            match.tB.showbo();
        }

        cout << "\nFinal Result \n";
        BaseStats* bs;

        if (match.tA.tr > match.tB.tr) {
            bs = &match.tA;
            int margin = match.tA.tr - match.tB.tr;
            bs->winner(match.tA.name, margin);
        } 
        else if (match.tB.tr > match.tA.tr) {
            bs = &match.tB;
            int margin = match.tB.tr - match.tA.tr;
            bs->winner(match.tB.name, margin);
        } 
        else {
            bs = &match.tA;
            bs->winner();
        }

        manOfTheMatch(match.tA, match.tB);

        cout << "\nSummary \n";
        BaseStats* summary;

        summary = &match.tA;
        summary->showStats();

        summary = &match.tB;
        summary->showStats();

    } catch (const invalid_argument&) {
        cout << "Invalid input detected.\n";
        return 1;
    } catch (const exception&) {
        cout << "An unexpected error occurred.\n";
        return 1;
    }


    return 0;
}
