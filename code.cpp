#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <exception>
using namespace std;

class Player
{
public:
    string name;
    int r, b, four, six, w, ov, con;

    Player(string n = "")
    {
        this->name = n;
        this->r = this->b = this->four = this->six = this->w = this->ov = this->con = 0;
    }

    inline int strikeRate()
    {
        if (this->b == 0)
        {
            return 0;
        }
        return (this->r * 100.0) / this->b;
    }

    inline int economy()
    {
        if (this->ov == 0)
        {
            return 0;
        }
        return this->con / (int)this->ov;
    }

    Player &operator++()
    {
        this->b++;
        return *this;
    }

    void addBall()
    {
        this->b++;
    }
    void addConceded(int runs)
    {
        this->con += runs;
    }
};

class Batsman : public Player
{
public:
    Batsman(string n = "") : Player(n) {}
};

class Bowler : public Player
{
public:
    Bowler(string n = "") : Player(n) {}
};

class Team : public Player
{
public:
    Player *pl;
    int cnt;
    int tr, wkt, ob, bb;

    Team(string n = "", int pCnt = 0) : Player(n)
    {
        this->cnt = pCnt;
        this->tr = this->wkt = this->ob = this->bb = 0;
        if (pCnt > 0)
        {
            try
            {
                this->pl = new Player[pCnt];
            }
            catch (bad_alloc &e)
            {
                cout << " error: " << endl;
                this->pl = nullptr;
                this->cnt = 0;
            }
        }
        else
        {
            this->pl = nullptr;
        }
    }

    void addPlayer(string &pname, int idx)
    {
        this->pl[idx] = Player(pname);
    }

    void showb()
    {
        cout << "\nBatting " << this->name << "\n";
        cout << "Name" << "  " << "Runs" << "  " << "Balls" << "  " << "4s" << "  " << "6s" << "  " << "SR" << "\n";
        for (int i = 0; i < this->cnt; ++i)
        {
            cout << this->pl[i].name << "  " << this->pl[i].r << "  " << this->pl[i].b << "  " << this->pl[i].four << "  " << this->pl[i].six << "  " << this->pl[i].strikeRate() << "\n";
        }
    }

    void showbo()
    {
        cout << "\nBowling " << this->name << "\n";
        cout << "Name" << "  " << "Overs" << "  " << "Runs" << "  " << "Wkts" << "  " << "Eco" << "\n";
        for (int i = 0; i < this->cnt; ++i)
        {
            if (this->pl[i].ov > 0)
            {
                cout << this->pl[i].name << "  " << this->pl[i].ov << "  " << this->pl[i].con << "  " << this->pl[i].w << "  " << this->pl[i].economy() << "\n";
            }
        }
    }

    ~Team()
    {
        delete[] pl;
    }
};

class Match : public Team
{
public:
    Team tA, tB;
    int ov;
    int str, ns, bow;

    Match(string a, string b, int o, int pCnt)
        : Team("Match", 0), tA(a, pCnt), tB(b, pCnt)
    {
        ov = o;
        str = ns = bow = -1;
    }

    void sti(Team &bat, Team &bowl, bool is2nd = false, int tgt = 0, int n = 0);
    void upsc(Team &bat, Team &bowl, int evt);
    bool checkEnd(Team &bat, Team &bowl, int tgt, int n);
};