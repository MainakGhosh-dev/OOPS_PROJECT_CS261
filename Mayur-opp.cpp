void Match::upsc(Team &bat, Team &bowl, int evt) {
    try {
        Player &batter = bat.pl[this->str];
        Player &bowler = bowl.pl[this->bow];
        int runs = 0;

        if (evt >= 0 && evt <= 6) {
            batter.r += evt;
            ++batter;
            bat.tr += evt;
            bowler.addConceded(evt);
            if (evt == 4) batter.four++;
            if (evt == 6) batter.six++;
            runs = evt;
        } else if (evt == 7) {
            bat.tr += 1;
            bowler.addConceded(1);
        } else if (evt == 9) {
            bat.wkt++;
            ++batter;
            bowler.w++;
            if (bat.wkt < bat.cnt - 1) {
                do {
                    cout << "new batsman index: ";
                    cin >> this->str;
                    if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
                        cin.clear(); cin.ignore(10, '\n');
                        cout << "invalid index\n";
                        this->str = -1;
                    }
                } while (this->str == -1);
            }
        } else if (evt == 10 || evt == 11) {
            cout << "Enter runs: ";
            cin >> runs;
            if (!cin) {
                cin.clear();
                cin.ignore(10, '\n');
                runs = 0;
                cout << "invalid\n";
            }
            bat.tr += runs;
            bowler.addConceded(runs);
        }

        if (evt != 7) {
            bat.bb++;
            if (bat.bb % 6 == 0) {
                bat.ob++;
                bowler.ov++;
                swap(this->str, this->ns);
            } else if (runs % 2 != 0) {
                swap(this->str, this->ns);
            }
        }
    } catch (exception& e) {
        cout << "Error" << endl;
    }
}
