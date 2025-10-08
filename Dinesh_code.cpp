bool Match::checkEnd(Team &bat, Team &bowl, int tgt, int n)
{
    if (bat.tr > tgt)
    {
        cout << "\nResult\n"
             << bat.name << " won by " << (n - bat.wkt - 1) << " wickets\n";
        return true;
    }
    if (bat.wkt >= n - 1 && bat.tr <= tgt)
    {
        cout << "\nResult\n"
             << bowl.name << " won by " << (tgt - bat.tr) << " runs\n";
        return true;
    }
    return false;
}

int main()
{
    try
    {
        string t1, t2;
        int ov, n, fb;
        cout << "Team A name: ";
        getline(cin, t1);
        cout << "Team B name: ";
        getline(cin, t2);
        cout << "Total overs: ";
        cin >> ov;
        if (!cin || ov <= 0)
        {
            cout << "Invalid\n";
            ov = 1;
            cin.clear();
            cin.ignore(10, '\n');
        }

        cout << "Players per team: ";
        cin >> n;
        if (n <= 1)
        {
            cout << "Invalid\n";
            n = 2;
        }
        cin.ignore();

        Match match(t1, t2, ov, n);

        cout << "\nTeam A players:\n";
        for (int i = 0; i < n; i++)
        {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tA.addPlayer(name, i);
        }

        cout << "\nTeam B players:\n";
        for (int i = 0; i < n; i++)
        {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tB.addPlayer(name, i);
        }

        cout << "\nbat first (1=" << t1 << ", 2=" << t2 << "):";
        cin >> fb;
        if (fb != 1 && fb != 2)
        {
            cout << "invalid.\n";
            fb = 1;
        }

        if (fb == 1)
        {
            match.sti(match.tA, match.tB);
            match.tA.showb();
            match.tB.showbo();
            match.sti(match.tB, match.tA, true, match.tA.tr, n);
            match.tB.showb();
            match.tA.showbo();
        }
        else
        {
            match.sti(match.tB, match.tA);
            match.tB.showb();
            match.tA.showbo();
            match.sti(match.tA, match.tB, true, match.tB.tr, n);
            match.tA.showb();
            match.tB.showbo();
        }

        cout << "\nFinal Result \n";
        BaseStats *bs;
        if (match.tA.tr > match.tB.tr)
        {
            bs = &match.tA;
            bs->announceWinner(match.tA.name);
        }
        else if (match.tB.tr > match.tA.tr)
        {
            bs = &match.tB;
            bs->announceWinner(match.tB.name);
        }
        else
        {
            cout << "Draw\n";
        }

        cout << "\nPost-Match Summary \n";
        BaseStats *summary;

        summary = &match.tA;
        summary->showStats();

        summary = &match.tB;
        summary->showStats();

        int sampleIndex = 0;
        summary = &match.tA.pl[sampleIndex];
        summary->showStats();
    }
    catch (const exception &e)
    {
        cout << "Error occurred\n";
        return 1;
    }
    return 0;
}
