#pragma once

class TommyCounter {
private:
    string input;
	int Counter = 0;
    int NumberOfEnters = 0;

public:

	TommyCounter(string Input) {
		input = Input;
	}

    //Counting Commas in a string even when there are /n (enters)
	int CountCommas() {
        int Counter = 0;
        NumberOfEnters = 0;
        while (input.length() != 0) {
            NumberOfEnters++;
            for (int i = 0; i < input.size(); i++) {
                if (input[i] == ',')Counter++;
            }
            cout << "/n" << endl;
            getline(cin, input);
        }
        return Counter;
	}

    int getNumberOfEnters() {
        return NumberOfEnters;
    }
};

/*

    Scott.Paton@everwarmgroup.com,
    r.h.jcompany@gmail.com,
    b.kiernan@dacoll.co.uk,
    jmitchell@combserve.co.uk,
    Davidrodger@sibcas.co.uk,
    ashelectrical@outlook.com,
    ecrabbie@boydbrothers.co.uk,
    enliteelectrical20@gmail.com,
    admin@response-bms.co.uk,
    yestradesltd@gmail.com,
    dchadwick.ces@gmail.com,
    Rclose11810@sky.com,
    crkerr@hotmail.co.uk,
    info@tttesting.co.uk,
    david.newton@westlothian.gov.uk,
    clanelectrical@gmail.com,
    info@loganlimited.co.uk,
    david.graham31@hotmail.co.uk,
    info@acwelectrical.solutions,
    Mathew.McClurg@mearsgroup.co.uk,
    martin.tutt@scomacuk.com,
    allbrightelectrical@hotmail.com,
    markheaney@heaneys.co.uk,
    andy.russell@livingstonme.co.uk,
    admin@tileyelectrical.co.uk,
    electrical@techsureuk.com,
    dmelectrical2016@gmail.com,
    info@e2cbms.com,
    info@rmdelectrical.co.uk,
    anne.riddell@integral.co.uk,
    Mail@plus5maintenance.co.uk,
    devon@electroforceandco.com,
    conor_mc_gowan@hotmail.com,
    scampbellelectrical@hotmail.com,
    allin1propertysolutions@hotmail.co.uk,
    nick.mclaren@fieldandlawn.com,
    accounts@bcagroup.org,
    david.mccarron@boulting.co.uk,
    finance@bmm-ltd.com,
    nickygecko@aol.com,
    wayne@stewartelectrical.co.uk,
    annemerie.gardiner@falkirk.gov.uk,
    ssoroka@edinmoreltd.co.uk,
    stephen.smith@emtecgroup.co.uk,
    lpsbsltd@outlook.com,
    gaztop1969@gmail.com,
    hsielectrical1@hotmail.com,
    gordon.mccomb@yahoo.com,
    hammaltonelect@btinternet.com,
    electrical@scsgroup.co.uk,
    keithmiddlemass@hotmail.com,
    admin@walmarelectrical.co.uk,
    mackieelectricalsolutions@outlook.com,
    aytonelectrical@outlook.com,
    eletracall@outlook.com,
    dave@energysource.uk.com

*/