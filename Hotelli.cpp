#include <iostream>
#include <windows.h>

using namespace std;


//-----------------------------------------------------------
//###-Globaalit vakiot-###

//Huoneita on v‰lilt‰ 101 - 400. Eli huoneita on 300. T‰m‰ toimii myˆs huoneiden maksimim‰‰r‰n‰
const int HUONEMAARA = 300;

//Minimim‰‰r‰ vapaita huoneita
const int MINVAPAITA = 40;

//Huonehinnat
const float YKSIHUONEHINTA = 100;
const float KAKSIHUONEHINTA = 150;

//Varausnumerot ovat v‰lilt‰ 10000 - 99999
const int MAXVARAUSNUM = 99999;
const int MINVARAUSNUM = 10000;

//N‰in monta eri‰ ruokalajia valmistetaan hotellissa
const int RUOKAMAARA = 30;


//-----------------------------------------------------------
//###-Structit-###

struct Huone
{
    int huoneNum;
    bool kahdenHengen;
    bool varattu;

    int varausNum;
    string etunimi;
    string sukunimi;
};



//-----------------------------------------------------------
//###-Aliohjelmat-###

Huone GeneroiHuone(int&, bool&);

Huone UusiHuoneVaraus(int&, bool&);

void kuittiTulostin(int&, int&, bool&, string&, string&);

void TulostaRuokalista(int&, int&, int&);

void TietoaHotellista();

int SyoteTarkistinValinta(int, int);

string SyoteTarkistinString(string);



//-----------------------------------------------------------
//###-P‰‰ohjelma-###
void main()
{
	//laitetaan kieli Suomeksi, ett‰ ‰‰kkˆset toimivat
    setlocale(LC_ALL, "fi-FI");

    //Laitetaan t‰m‰, ett‰ ‰‰kkˆset toimii ciniss‰
    SetConsoleCP(1252);


	//Tehd‰‰n t‰m‰ srand komento, ett‰ satunnaisesti generoivat numerot toimisivat
    //Muistutus, ett‰ rand toimii t‰lleen: rand() % (MAXRAND - MINRAND + 1) + MINRAND;
	srand(time(0));



    //-----------------------------------------------------------
    //###-Muuttujat-### (Vain osa niist‰. Alempaa lˆytyy kyll‰ lis‰‰kin)

    int ruokaNumerot[3]; //T‰h‰n muuttujaan laitetaan ruokaNumerot, joita k‰ytet‰‰n ruokalistassa

    bool ohjelmaPaalla = true; //T‰m‰ kertoo onko p‰‰ohjelma p‰‰ll‰

    int valinta; //P‰‰ohjelman ensimm‰inen valinta

    int valintaHuoneKoko; //Huoneen koon valinta

    int valintaSatunnainen; //Huoneen satunnaisuuden tai valitsemisen valinta

    int valintaYotHotellissa; //Yˆt hotellissa valinta

    Huone hotelliHuone[HUONEMAARA]; //struct hotellihuoneista


    //Generoidaan ruokanumerot
    for (int i = 0; i < 3; i++)
    {
        //T‰ss‰ on numerot nollan ja 29 v‰lilt‰, koska on 30 eri ruokalajia
        ruokaNumerot[i] = rand() % (29 - 0 + 1) + 0;

        //T‰m‰n sis‰ll‰ on loop, joka tarkistaa, ett‰ samoja numeroita ei ilmesty
        if (i > 0)
        {
            for (int k = 1; k <= i; k++)
            {
                if (ruokaNumerot[i] == ruokaNumerot[i - k])
                {
                    i--;
                    break;
                }
            }
        }
    }

    //T‰ss‰ kohtaa generoidaan vapaat huoneet v‰lilt‰ 40-300

    /*
    T‰ss‰ bool arrayss‰ false tarkoittaa sit‰, ett‰ huone on vapaa. Jos taas arrayn kohta on true,
    niin generoidaan siihen huonenumeroon asiakas.
    */
    bool generoiSatunnainenHuone[HUONEMAARA];

    //Laitetaan siihen ekaks kaikki trueksi
    for (int i = 0; i < HUONEMAARA; i++)
    {
        generoiSatunnainenHuone[i] = true;
    }

    //Sitten generoidaan se vapaiden huoneiden m‰‰r‰ v‰lilt‰ 40-300
    int vapaaHuoneMaara = rand() % (HUONEMAARA - MINVAPAITA + 1) + MINVAPAITA;

    //Jos ei ole parillinenluku, niin tehd‰‰n siit‰ parillinenluku
    if (vapaaHuoneMaara % 2 != 0)
    {
        vapaaHuoneMaara++;
    }


    //Sitten muutama muuttuja mit‰ tarvitaan seuraavassa for loopissa
    int satunnainenHuone;
    bool kirjoita;

    //T‰m‰ for looppi toistuu niin monta kertaa kuin vapaita huoneita on
    for (int i = 0; i < vapaaHuoneMaara; i++)
    {
        //Laitetaan alussa kirjoita todeksi ja generoidaan satunnainen huonenumero
        kirjoita = true;

        //T‰ss‰ miinustetaan randista 1, ett‰ saadaan 0-299
        satunnainenHuone = (rand() % (HUONEMAARA - 1 + 1) + 1) - 1;


        //T‰m‰ if menee l‰pi ensimm‰isen kierroksen j‰lkeen
        //Sen sis‰ll‰ on loop, joka tarkistaa, ett‰ samoja numeroita ei ilmesty
        if (i > 0)
        {
            //Jos arrayn kohta on false, niin siin‰ on jo k‰ynyt joku numero
            if (generoiSatunnainenHuone[satunnainenHuone] == false)
            {
                //Jos joku numero on k‰ynyt siin‰ kohdassa, niin menn‰‰n i;ss‰ takaisin
                //for looppi siis toistuu siihen asti, ett‰ numero saadaan generoitua
                i--;
                kirjoita = false;
            }
        }

        //T‰m‰ tapahtuu, jos arrayn kohdalla ei ole k‰ynyt jo numeroa
        if (kirjoita == true)
        {
            generoiSatunnainenHuone[satunnainenHuone] = false;
        }
    }


    //For looppi, jossa luodaan huoneet ja generoidaan "feikki varaukset"
    for (int i = 0; i < HUONEMAARA; i++)
    {
        //Jos generoiSatunnainenHuone[i] on true, niin huoneeseen laitetaan asukas
        hotelliHuone[i] = GeneroiHuone(i, generoiSatunnainenHuone[i]);

        //Debugaamiseen k‰ytetty generoitujen huoneiden tulostus
        //Jos opettaja haluaa n‰hd‰ generoidut huoneet hyvin, niin kannattaa ottaa kommentit pois
        //Toinen kommentin p‰‰ lˆytyy rivilta 217
        /*
        if (hotelliHuone[i].varattu == true)
        {
            cout << hotelliHuone[i].huoneNum << " on varattu. ";
            cout << "Sen varaaja on " << hotelliHuone[i].etunimi << " " << hotelliHuone[i].sukunimi;
            
            if (hotelliHuone[i].kahdenHengen == true)
            {
                cout << ". Huone on kahden hengen huone.";
            }
            else
            {
                cout << ". Huone on yhden hengen huone.";
            }

            cout << " Varausnumero: " << hotelliHuone[i].varausNum << endl;
        }
        else 
        {
            cout << hotelliHuone[i].huoneNum << " ei ole varattu.";

            if (hotelliHuone[i].kahdenHengen == true)
            {
                cout << " Huone on kahden hengen huone." << endl;
            }
            else
            {
                cout << " Huone on yhden hengen huone." << endl;
            }
        }
        */
        
    }
    
    //Tervehdit‰‰n
    cout << "################################################" << endl;
    cout << "##-Tervetuloa Hotelli Kullervon sovellukseen!-##" << endl;
    cout << "################################################" << endl;


    //P‰‰valikko
    while (ohjelmaPaalla == true)
    {
        cout << endl << "---------------------------------------------------------------" << endl;
        cout << "Mit‰ haluaisit tehd‰? (Syˆta numero 1-5)" << endl;
        cout << "1. Varaa huone" << endl;
        cout << "2. N‰yt‰ ruokalista" << endl;
        cout << "3. Tietoa hotellista" << endl;
        cout << "4. Etsi varaus" << endl;
        cout << "5. Poistu" << endl;

        valinta = SyoteTarkistinValinta(5, 1);


        /*
        Kauhean pitk‰ switch case.
        Case 1 rivill‰: 251 
        Case 2 rivill‰: 573
        Case 3 rivill‰: 581
        Case 4 rivill‰: 589
        Case 5 rivill‰: 680
        */
        switch (valinta)
        {
        case 1:
        {
            //T‰ss‰ casessa varataan huone

            //Otetaan ylˆs haluaako yhden vai kahden hengen huoneen
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Tervetuloa huoneen varaukseen!" << endl;
            cout << "Jos haluat yhdenhengen huoneen, syˆt‰ luku 1. (Hinta per yˆ 100e)" << endl;
            cout << "Jos haluat kahdenhengen huoneen, syˆt‰ luku 2. (Hinta per yˆ 150e)" << endl;
            cout << "Jos haluat poistua, syˆt‰ luku 3." << endl << endl;

            valintaHuoneKoko = SyoteTarkistinValinta(3, 1);


            //tarkistetaan pit‰‰kˆ poistua
            if (valintaHuoneKoko == 3) break;


            //Tarkistetaan onko huoneita vapaana. Laitetaan ensin huoneitaVapaana falselle
            bool huoneitaVapaana = false;

            //T‰m‰ tapahtuu, jos valitsi yhden hengen huoneen
            if (valintaHuoneKoko == 1)
            {
                for (int i = 0; i < HUONEMAARA; i++)
                {
                    //Jos yht‰‰n huonetta ei ole varattu ja se on yhdenhengenhuone tehd‰‰n n‰in
                    if (hotelliHuone[i].varattu == false && hotelliHuone[i].kahdenHengen == false)
                    {
                        huoneitaVapaana = true;
                    }
                }
            }


            //T‰m‰ tapahtuu, jos valitsi kahden hengen huoneen
            if (valintaHuoneKoko == 2)
            {
                for (int i = 0; i < HUONEMAARA; i++)
                {
                    //Jos yht‰‰n huonetta ei ole varattu ja se on kahdenhengenhuone tehd‰‰n n‰in
                    if (hotelliHuone[i].varattu == false && hotelliHuone[i].kahdenHengen == true)
                    {
                        huoneitaVapaana = true;
                    }
                }
            }


            //Jos huoneita ei ole vapaana, niin l‰hdet‰‰n pois
            if (huoneitaVapaana == false)
            {
                cout << "Huoneita joita etsit ei valitettavasti ole vapaana." << endl;
                break;
            }



            //Otetaan ylˆs haluaako huoneen satunnaisesti vai valita sen
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Haluatko huoneen satunnaisesti vai haluatko valita sen itse?" << endl;
            cout << "Jos haluat satunnaisen huoneen, syˆt‰ luku 1." << endl;
            cout << "Jos haluat valita huoneen, syˆt‰ luku 2." << endl;
            cout << "Jos haluat poistua, syˆt‰ luku 3." << endl << endl;

            valintaSatunnainen = SyoteTarkistinValinta(3, 1);

            //tarkistetaan pit‰‰kˆ poistua
            if (valintaSatunnainen == 3) break;



            //Otetaan ylˆs kuinka monta yˆt‰ haluaa yˆpy‰
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Syˆt‰ kuinka monta yˆt‰ haluat yˆpy‰ hotellissa. (Max syˆte: 100)" << endl;
            cout << "Ota asiakaspalveluun yhteytt‰, jos olet varaamassa huonetta yli sadaksi p‰iv‰ksi!" << endl;
            cout << "Jos haluat poistua, syˆt‰ luku 0." << endl << endl;

            valintaYotHotellissa = SyoteTarkistinValinta(100, 0);

            //tarkistetaan pit‰‰kˆ poistua
            if (valintaYotHotellissa == 0) break;


            //Luodaan hinta muuttuja ylˆs
            float hinta = 0;

            //Lasketaan hinta sen perusteella kuinka monta yˆt‰ on hotellissa
            if (valintaHuoneKoko == 1)
            {
                hinta = valintaYotHotellissa * YKSIHUONEHINTA;
            }
            else
            {
                hinta = valintaYotHotellissa * KAKSIHUONEHINTA;
            }


            //Else if siit‰, ett‰ saako alennusta. Noin 20% mahollisuus molemmissa
            if (rand() % (5 - 1 + 1) + 1 == 5)
            {
                //20% alennus
                hinta = hinta * 0.8;

                //Tulostetaan
                cout << endl << "---------------------------------------------------------------" << endl;
                cout << "Onnittelut! Sait 20% alennusta!" << endl;
                cout << "Huoneen hinta on: " << hinta << " euroa." << endl << endl;
            }
            else if (rand() % (5 - 1 + 1) + 1 == 5)
            {
                //10% alennus
                hinta = hinta * 0.9;

                //Tulostetaan
                cout << endl << "---------------------------------------------------------------" << endl;
                cout << "Onnittelut! Sait 10% alennusta!" << endl;
                cout << "Huoneen hinta on: " << hinta << " euroa." << endl << endl;
            }
            else
            {
                //Tulostetaan
                cout << endl << "---------------------------------------------------------------" << endl;
                cout << "Huoneen hinta on: " << hinta << " euroa." << endl << endl;
            }

            //Tarkistetaan kuulostaako hinta hyv‰lt‰
            cout << "N‰ytt‰‰kˆ huoneen hinta hyv‰lt‰?" << endl;
            cout << "Jos n‰ytt‰‰, niin syˆt‰ 1. P‰‰set sitten t‰ytt‰m‰‰n varauslomakkeen." << endl;
            cout << "Jos ei, niin syˆt‰ 2. T‰m‰ on viimeinen hetki milloin voi peruuttaa." << endl;

            //L‰hdet‰‰n pois, jos kakkoneen syˆtet‰‰n. Jos ykkˆnen sˆytet‰‰n, niin ei tehd‰ mit‰‰n
            if (SyoteTarkistinValinta(2, 1) == 2) break;



            //T‰m‰ kauhea if else hirviˆ vie sinut huoneen varauslomakkeelle
            //Jos halusi satunnaisen huoneen
            if (valintaSatunnainen == 1)
            {
                //Jos on yhdenhengen huone, niin tehd‰‰n n‰in
                if (valintaHuoneKoko == 1)
                {
                    //T‰m‰ toistuu siihen asti, ett‰ huone lˆytyy
                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        //T‰st‰ menee l‰pi vain, jos huone on yhdenhengen ja vapaa
                        if (hotelliHuone[i].kahdenHengen == false && hotelliHuone[i].varattu == false)
                        {
                            //T‰ss‰ on se "arvonta". 1/5 mahdollisuus, ett‰ huone tulee valituksi.
                            if (rand() % (4 - 0 + 1) + 0 == 0)
                            {
                                //Otetaan k‰ytt‰j‰lt‰ h‰nen tietonsa
                                hotelliHuone[i] = UusiHuoneVaraus(hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen);

                                //Tulostetaan kuitti
                                kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                                //L‰het‰‰n pakoon t‰st‰ kauheasta for loopista
                                break;
                            }
                        }

                        //T‰m‰ resettaa loopin, jos arvonta ei osu huoneeseen
                        if (i == HUONEMAARA - 1)
                        {
                            i = 0;
                        }
                    }

                    //Palataan alkuvalikkoon
                    break;
                }

                //Jos ei ollut yhdenhengen huone, niin on kahdenhengen huone. Tehd‰‰n n‰in
                else
                {
                    //T‰m‰ toistuu siihen asti, ett‰ huone lˆytyy
                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        //T‰st‰ menee l‰pi vain, jos huone on kahdenhengen ja vapaa
                        if (hotelliHuone[i].kahdenHengen == true && hotelliHuone[i].varattu == false)
                        {
                            //T‰ss‰ on se "arvonta". 1/5 mahdollisuus, ett‰ huone tulee valituksi.
                            if (rand() % (4 - 0 + 1) + 0 == 0)
                            {
                                //Otetaan k‰ytt‰j‰lt‰ h‰nen tietonsa
                                hotelliHuone[i] = UusiHuoneVaraus(hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen);

                                //Tulostetaan kuitti
                                kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                                //L‰het‰‰n pakoon t‰st‰ kauheasta for loopista
                                break;
                            }
                        }

                        //T‰m‰ resettaa loopin, jos arvonta ei osu huoneeseen
                        if (i == HUONEMAARA - 1)
                        {
                            i = 0;
                        }
                    }

                    //Palataan alkuvalikkoon
                    break;
                }
            }
            //T‰m‰ else tapahtuu, jos valinta ei ole satunnainen
            else
            {
                //Boolian, jolla tiedet‰‰n onnistuiko huoneen valitseminen
                bool huoneValittu = false;

                //Jos on yhdenhengen huone, niin tehd‰‰n n‰in
                if (valintaHuoneKoko == 1)
                {
                    //Tulostetaan t‰m‰ teksti jonka j‰lkeen loopilla tulostetaan vapaat yhdenhengen huoneet
                    cout << endl << "---------------------------------------------------------------" << endl;
                    cout << "Seuraavat yhdenhengen huoneet ovat vapaina: " << endl;

                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        if (hotelliHuone[i].varattu == false && hotelliHuone[i].kahdenHengen == false)
                        {
                            cout << "-" << hotelliHuone[i].huoneNum << endl;
                        }
                    }


                    //T‰ll‰ while loopilla valitaan huone
                    while (huoneValittu == false)
                    {
                        //Pyydet‰‰n ensin huonenumeroa mit‰ k‰ytt‰j‰ haluaa ja otetaan se muuttujaan
                        cout << endl << "Syˆt‰ haluamasi huonenumero." << endl;
                        int huoneNumeroValinta = SyoteTarkistinValinta(400, 101);

                        //Sitten etsit‰‰n for loopin avulla se huone, mit‰ halutaan
                        for (int i = 0; i < HUONEMAARA; i++)
                        {
                            //Jos huone ei ole varattu, sill‰ on oikea numero ja se on yhdenhengen huone
                            if (hotelliHuone[i].varattu == false && hotelliHuone[i].huoneNum == huoneNumeroValinta && hotelliHuone[i].kahdenHengen == false)
                            {
                                //Otetaan k‰ytt‰j‰lt‰ h‰nen tietonsa
                                hotelliHuone[i] = UusiHuoneVaraus(hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen);

                                //Tulostetaan kuitti
                                kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                                //Kerrotaan ohjelmalle, ett‰ huone lˆytyi
                                huoneValittu = true;

                                //L‰hdet‰‰n pois t‰st‰ for loopista
                                break;
                            }
                        }

                        //Jos for loop ei saavuttanut oikeaa huonetta, niin tulostetaan t‰m‰
                        if (huoneValittu == false)
                        {
                            cout << "Et valinnut listalta oikeaa huonetta" << endl;
                        }
                    }

                }
                //Jos ei ollut yhdenhengen huone, niin on kahdenhengen huone. Tehd‰‰n n‰in
                else
                {
                    //Tulostetaan t‰m‰ teksti jonka j‰lkeen loopilla tulostetaan vapaat kahdenhengen huoneet
                    cout << endl << "---------------------------------------------------------------" << endl;
                    cout << "Seuraavat kahdenhengen huoneet ovat vapaina: " << endl;

                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        if (hotelliHuone[i].varattu == false && hotelliHuone[i].kahdenHengen == true)
                        {
                            cout << "-" << hotelliHuone[i].huoneNum << endl;
                        }
                    }


                    //T‰ll‰ while loopilla valitaan huone
                    while (huoneValittu == false)
                    {
                        //Pyydet‰‰n ensin huonenumeroa mit‰ k‰ytt‰j‰ haluaa ja otetaan se muuttujaan
                        cout << endl << "Syˆt‰ haluamasi huonenumero." << endl;
                        int huoneNumeroValinta = SyoteTarkistinValinta(400, 101);

                        //Sitten etsit‰‰n for loopin avulla se huone, mit‰ halutaan
                        for (int i = 0; i < HUONEMAARA; i++)
                        {
                            //Jos huone ei ole varattu, sill‰ on oikea numero ja se on kahdenhengen huone
                            if (hotelliHuone[i].varattu == false && hotelliHuone[i].huoneNum == huoneNumeroValinta && hotelliHuone[i].kahdenHengen == true)
                            {
                                //Otetaan k‰ytt‰j‰lt‰ h‰nen tietonsa
                                hotelliHuone[i] = UusiHuoneVaraus(hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen);

                                //Tulostetaan kuitti
                                kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                                //Kerrotaan ohjelmalle, ett‰ huone lˆytyi
                                huoneValittu = true;

                                //L‰hdet‰‰n pois t‰st‰ for loopista
                                break;
                            }
                        }

                        //Jos for loop ei saavuttanut oikeaa huonetta, niin tulostetaan t‰m‰
                        if (huoneValittu == false)
                        {
                            cout << "Et valinnut listalta oikeaa huonetta!" << endl;
                        }
                    }
                }

            }

            //L‰hdet‰‰n vihdoin ulosta t‰st‰ pitk‰sta casesta
            break;
        }
            //--------------------------------------------------------
        case 2:
        {
            //T‰m‰ vain tulostaa ruokalistan niiden numeroiden perusteella
            TulostaRuokalista(ruokaNumerot[0], ruokaNumerot[1], ruokaNumerot[2]);

            break;
        }
            //--------------------------------------------------------
        case 3:
        {
            //T‰ss‰ k‰yd‰‰n TietoaHotellista aliohjelmassa
            TietoaHotellista();

            break;
        }
            //--------------------------------------------------------
        case 4:
        {
            //T‰‰ll‰ etsit‰‰n varauksia

            //muuttujat

            bool varausLoydettu; //Boolian, jolla tiedet‰‰n onnistuiko varauksen lˆyt‰minen
            bool etsiVarauksia = true; //Boolian, jolla tiedet‰‰n pit‰‰kˆ varauksia viel‰ etsi‰
            int etsiValinta; //Int jolla valitaan miten etsit‰‰n

            //Looppi pyˆrii siihen asti, ett‰ ei en‰‰ halua etsi‰ varauksia
            while (etsiVarauksia == true)
            {
                //T‰m‰ pit‰‰ aina laittaa falseksi alussa
                varausLoydettu = false;

                //Tulostetaan n‰m‰ aina while loopin alussa
                cout << endl << "---------------------------------------------------------------" << endl;
                cout << "Olet etsim‰ss‰ varausta." << endl;
                cout << "Etsiv‰ sovellus tulostaa kaikki kuitit miss‰ on sama syˆte." << endl;
                cout << "Etsint‰ onnistuu kahdella tavalla." << endl << endl;
                cout << "Jos haluat etsi‰ varauksia varausnumerolla, syˆt‰ 1." << endl;
                cout << "Jos haluat etsi‰ varauksia sukunimell‰, syˆt‰ 2." << endl;
                cout << "Jos haluat poistua, syˆt‰ 3." << endl;

                etsiValinta = SyoteTarkistinValinta(3, 1);

                //Jos halutaan poistua, niin poistutaan
                if (etsiValinta == 3) break;


                //Jos halutaan etsi‰ varausnumerolla niin tehd‰‰n t‰m‰
                if (etsiValinta == 1)
                {
                    //Pyydet‰‰n ensin varausnumeroa ja otetaan se muuttujaan
                    cout << endl << "Syˆt‰ etsim‰si varausnumero." << endl;
                    int etsiVarausNumero = SyoteTarkistinValinta(MAXVARAUSNUM, MINVARAUSNUM);

                    //Sitten etsit‰‰n for loopin avulla se huone, mit‰ halutaan
                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        if (hotelliHuone[i].varausNum == etsiVarausNumero)
                        {
                            //Tulostetaan kuitti
                            kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                            //Kerrotaan ohjelmalle, ett‰ huone lˆytyi
                            varausLoydettu = true;
                        }
                    }
                }
                //Jos t‰m‰ else tapahtuu, niin etsit‰‰n sukunimell‰
                else
                {
                    //Pyydet‰‰n ensin varausnumeroa ja otetaan se muuttujaan
                    cout << endl << "Syˆt‰ etsim‰si sukunimi." << endl;
                    string etsiVarausSukunimi = SyoteTarkistinString("sukunimi");

                    //Sitten etsit‰‰n for loopin avulla se huone, mit‰ halutaan
                    for (int i = 0; i < HUONEMAARA; i++)
                    {
                        if (hotelliHuone[i].sukunimi == etsiVarausSukunimi)
                        {
                            //Tulostetaan kuitti
                            kuittiTulostin(hotelliHuone[i].varausNum, hotelliHuone[i].huoneNum, hotelliHuone[i].kahdenHengen, hotelliHuone[i].etunimi, hotelliHuone[i].sukunimi);

                            //Kerrotaan ohjelmalle, ett‰ huone lˆytyi
                            varausLoydettu = true;
                        }
                    }
                }

                //Jos for loop ei saavuttanut oikeaa varausta, niin tulostetaan t‰m‰
                if (varausLoydettu == false)
                {
                    cout << "Emme valitettavasti lˆyt‰neet varausta." << endl;
                }

                //Kysyt‰‰n haluaako viel‰ etsi‰ varauksia
                cout << endl << "---------------------------------------------------------------" << endl;
                cout << "Syˆt‰ 1, jos haluat viel‰ etsi‰ varauksia." << endl;
                cout << "Syˆt‰ 2, jos haluat palata p‰‰valikkoon." << endl;

                //Jos ei halua, niin laitetaan etsivarauksia falseksi, joka pys‰ytt‰‰ whilen
                if (SyoteTarkistinValinta(2, 1) == 2) etsiVarauksia = false;
            }

            //Palataan p‰‰valikkoon
            break;
        }
            //--------------------------------------------------------
        case 5:
        {
            //T‰‰ll‰ poistutaan ohjelmasta
            ohjelmaPaalla = false;
            break;
        }
        }
    }

    //Hyv‰stell‰‰n
    cout << "################################################" << endl;
    cout << "#######-Hei hei ja tervetuloa uudelleen!-#######" << endl;
    cout << "################################################" << endl;

    return;
}


/*
Aliohjelma, joka numeroi huoneet ja generoi feikit huonevaraukset
Lˆytyy rivilt‰ 702
*/
Huone GeneroiHuone(int &huoneNumero, bool &onkoVarattu)
{
    //Tehd‰‰n struct, joka l‰hete‰‰n takaisin aliohjelman lopussa
    Huone luotuHuone;


    //Lis‰t‰‰n aina huonenumeroon 101
    //Sill‰ saadaat huoneita v‰lilt‰ 101-400
    luotuHuone.huoneNum = huoneNumero + 101;


    //Parillisista huoneista tehd‰‰n kahdenhengen huoneita
    if ((huoneNumero + 101) % 2 == 0)
    {
        luotuHuone.kahdenHengen = true;
    }
    else
    {
        luotuHuone.kahdenHengen = false;
    }


    //Onko huone varattu
    if (onkoVarattu == false)
    {
        luotuHuone.varattu = false;
    }
    else
    {
        luotuHuone.varattu = true;
    }


    //Jos generoitu huone on varattu, niin laitetaan siihen varausnumero, etunimi ja sukunimi
    if (luotuHuone.varattu == true)
    {
        //Generoidaan luodulle huoneelle varausnumero
        luotuHuone.varausNum = rand() % (MAXVARAUSNUM - MINVARAUSNUM + 1) + MINVARAUSNUM;


        //T‰ss‰ on etunimi‰, joita k‰ytet‰‰n generoitujen huoneiden varaukseen
        string etunimiPankki[30]{ "Matti", "Teppo", "Mortti", "Vertti", "Aku", "Roope",
                                "Pasi", "Masi", "Taru", "Siiri", "Saara", "Samuel", "Tapani",
                                "Suvi", "Maria", "Johanna", "Taneli", "Matias", "Viljami",
                                "Kristiina", "Aino", "Helmi", "Lilja", "Tero", "Sami", "Sauli",
                                "Aada", "Akseli", "Ossi", "Olivia" };

        //T‰ss‰ on sukunimi‰, joita k‰ytet‰‰n generoitujen huoneiden varaukseen
        string sukunimiPankki[30]{ "Koski", "Joki", "J‰rvi", "Puu", "Koivu", "Ranta", "T‰hti",
                                "M‰ki", "Punapuu", "Roihu", "Hakkarainen", "Aalto", "Aho", "Kangas",
                                "Kallio", "Kivi", "Rauta", "Karhu", "Haapala", "Saari", "Kuusi",
                                "Kauppila", "Kukkula", "Kirkkonen", "Kulkunen", "M‰kinen",
                                "Mets‰nen", "Koivunen", "Rautainen", "Koskinen" };


        //Haetaan etunimi ja sukunimi satunnisesti pankeista k‰ytt‰m‰l‰‰ randia
        luotuHuone.etunimi = etunimiPankki[rand() % (29 - 0 + 1) + 0];
        luotuHuone.sukunimi = sukunimiPankki[rand() % (29 - 0 + 1) + 0];
    }

    //Palautetaan luotuhuone
    return luotuHuone;
}



/*
Aliohjelma, jota k‰ytet‰‰n huoneen varaamiseen. Rivill‰ 771
*/
Huone UusiHuoneVaraus(int &huoneNumero, bool &kahdenhengenHuone)
{
    //Tehd‰‰n struct, joka l‰hete‰‰n takaisin aliohjelman lopussa
    Huone uusiHuone;

    //Laitetaan luotuun huoneeseen oikeat tiedot, joita ei tarvitse syˆtt‰‰
    uusiHuone.huoneNum = huoneNumero;
    uusiHuone.kahdenHengen = kahdenhengenHuone;
    uusiHuone.varattu = true;

    //Generoidaan luodulle huoneelle varausnumero
    uusiHuone.varausNum = rand() % (MAXVARAUSNUM - MINVARAUSNUM + 1) + MINVARAUSNUM;


    //Kysyt‰‰n etu ja sukunime‰
    cout << endl << "---------------------------------------------------------------" << endl;
    cout << "Huoneen varaus on melkein valmista! Tarvitsemme en‰‰ vain nimesi." << endl;

    uusiHuone.etunimi = SyoteTarkistinString("etunimesi");

    uusiHuone.sukunimi = SyoteTarkistinString("sukunimesi");


    //Palautetaan struct sinne mist‰ se kutsuttiin
    return uusiHuone;
}


/*
Aliohjelma, joka tulostaa kuitin. Rivill‰ 802
*/
void kuittiTulostin(int &varausnumero, int &huoneNumero, bool &kahdenHuone, string &etu, string &suku)
{
    cout << endl;
    cout << "####################################################" << endl;
    cout << "############-KUITTI HUONEEN VARAUKSESTA-############" << endl;
    cout << "####################################################" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Varausnumero: " << varausnumero << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Huoneenumero: " << huoneNumero << endl;
    cout << "----------------------------------------------------" << endl;

    if (kahdenHuone == true)
    {
        cout << "Kahdenhengen huone." << endl;
    }
    else
    {
        cout << "Yhdenhengen huone." << endl;
    }

    cout << "----------------------------------------------------" << endl;
    cout << "Varaajan nimi: " << etu << " " << suku << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Hotelli: Hotelli Kullervo" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "####################################################" << endl << endl;
}

/*
Aliohjelma, joka tulostaa ruokalistan alussa generoitujen numeroidan perusteella.
Rivill‰ 838.

Aliohjelmaan menev‰t numerot arvotaan jo ohjelma alussa, koska ruuat eiv‰t normaalisti vaihtele
joka kerta kun katsoo ruokalistaa.
*/
void TulostaRuokalista(int &ruoka1, int &ruoka2, int &ruoka3)
{
    //Tulostettavat ruuat
    string ruokalista[RUOKAMAARA]{ "Hernekeittoa", "Lohikeittoa", "Poronk‰ristyst‰", "Pizzabuffetin",
                    "Nakkikastiketta ja perunoita", "Porsaanpihve‰ ja ranskalaisia",
                    "Lehtipihve‰ ja lohkoperunoita", "Pinaattil‰ttyj‰ ja puolukkahilloa",
                    "Lihapullia ja perunamuusia", "Merimiespataa", "Kalakukkoa",
                    "Veril‰ttyj‰ ja puolukkahilloa", "Nugetteja ja ristikkoperunoita",
                    "Kebabbia ja ranskalaisia", "Kanaa ja riisi‰", "Makaroonilaatikkoa",
                    "Jauhelihakeittoa", "Naudan jauhelihapihve‰ ja perunoita",
                    "Tortillaa ja kaikenlaista t‰ytett‰", "Kanankoipia ja ranskalaisia",
                    "Pasta bolognesea", "Uunimakkaraa ja perunamuusia",
                    "Perunalaatikkoa", "Muikkuja ja perunamuusia", "Kaalik‰‰ryleit‰",
                    "Kaalilaatikkoa", "Karjalanpaistia", "H‰rk‰‰ kastikkeessa ja riisi‰",
                    "Kebabbia ja riisi‰", "Hirvikeittoa" };



    cout << "---------------------------------------------------------------" << endl;
    cout << "                    ##-P‰iv‰n ruokalista-##                    " << endl;
    cout << "---------------------------------------------------------------" << endl;

    cout << endl << "Jonosta yksi lˆyd‰t: " << ruokalista[ruoka1] << "." << endl << endl;

    cout << "---------------------------------------------------------------" << endl;

    cout << endl << "Jonosta kaksi lˆyd‰t: " << ruokalista[ruoka2] << "." << endl << endl;

    cout << "---------------------------------------------------------------" << endl;

    cout << endl << "Jonosta kolme lˆyd‰t: " << ruokalista[ruoka3] << "." << endl << endl;

    cout << "---------------------------------------------------------------" << endl;

    return;
}


/*
Aliohjelma, joka tulostaa haluamiasi tietoja hotellista. Rivill‰ 879
*/
void TietoaHotellista()
{
    //Tarvittavaat muuttujat
    bool jatkaKysyntaa = true;
    int kysymysValinta;


    //Tulostus
    cout << endl << "---------------------------------------------------------------" << endl;
    cout << "Tervetuloa hotelli Kullervon tietoihin! Mit‰ haluaisit tiet‰‰?" << endl << endl;


    //While loop, joka jatkuu siihen asti, ett‰ ei halua en‰‰ kysell‰
    while (jatkaKysyntaa == true)
    {
        //Kysyt‰‰n mit‰ haluaa tehd‰ ja otetaan syˆtteen tarkistimella syˆte
        cout << "Jos haluat tiet‰‰ perustiedot hotellista, syˆt‰ 1." << endl;
        cout << "Jos haluat tietoa ruokalasta ja yˆkerhosta, syˆt‰ 2." << endl;
        cout << "Jos haluat tietoa hotellin ymp‰ristˆst‰, syˆt‰ 3." << endl;
        cout << "Jos haluat hotellin yhteistiedot, syˆt‰ 4." << endl;
        cout << "Jos haluat poistua, syˆt‰ 5." << endl;

        kysymysValinta = SyoteTarkistinValinta(5, 1);


        //Switch case, joka vastaa kysymyksiin
        switch (kysymysValinta)
        {
        case 1:
        {
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Hotelli Kullervo on vuonna 1995 rakennettu hotelli." << endl;
            cout << "Se sijaitsee M‰kel‰n kaupungissa kullervontie ykkˆsess‰. " << endl;
            cout << "Hotellissa on nelj‰ kerrosta. Aula ja ruokala ovat nolla-" << endl;
            cout << "kerroksessa. Huoneet 100-199 ovat ensimm‰isess‰ kerroksessa." << endl;
            cout << "Huoneet 200-299 ovat toisessa kerroksessa. Kolmannessa" << endl;
            cout << "kerroksessa ovat huoneet 300-400." << endl;
            cout << endl << "---------------------------------------------------------------" << endl;
            
            break;
        }
        case 2:
        {
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Hotellistamme lˆytyy ruokala ja yˆkerho. Ruokalassa" << endl;
            cout << "on aamupala klo 7:30-10:30. P‰iv‰llinen on klo 15:00-18:00." << endl;
            cout << "Yokerho on ruokalassa auki klo 19:30-2:00. Yˆkerhossa myyd‰‰n" << endl;
            cout << "alkoholia, joten sinne p‰‰see vain t‰ysi-ik‰iset henkilˆt." << endl << endl;

            cout << "Lˆyd‰t ruokalan nollakerroksesta aulan vierest‰." << endl;
            cout << endl << "---------------------------------------------------------------" << endl;
            
            break;
        }
        case 3:
        {
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "Hotellin l‰helt‰ lˆytyy kauppakeskus M‰ki, kylpyl‰ Aqua," << endl;
            cout << "Lidl ja kuntosali Dynamite." << endl << endl;

            cout << "Hotellin l‰hell‰ on myˆs korkea m‰ki, jossa talvisin" << endl;
            cout << "voi lasketella ja kes‰isin siell‰ on karting rata." << endl;
            cout << endl << "---------------------------------------------------------------" << endl;
            
            break;
        }
        case 4:
        {
            cout << endl << "---------------------------------------------------------------" << endl;
            cout << "######################################" << endl;
            cout << "###-HOTELLI KULLERVON YHTEISTIEDOT-###" << endl;
            cout << "######################################" << endl << endl;

            cout << "S‰hkˆposti: hotelKullevo@gmail.com" << endl << endl;
            cout << "Puhelinnumero: 04 234 34246" << endl << endl;
            cout << "Osoite: Kullervontie 1, 35900, M‰kel‰" << endl;
            cout << endl << "---------------------------------------------------------------" << endl;

            break;
        }
        case 5:
        {
            //L‰hdet‰‰n ulosta while t‰st‰ loopista
            jatkaKysyntaa = false;
            break;
        }
        }

    }

    //Palataan p‰‰ohjelmaan
    return;
}


/*
###########################################
    !!Syˆtteen tarkistimet t‰m‰n alla!!
###########################################
*/


/*
Aliohjelma, joka tarkistaa intin syˆtteen. Rivill‰ 986
-Ensimm‰inen luku on isoin syˆtett‰v‰ arvo
-Toinen luku on pienin syˆtett‰v‰ arvo
*/
int SyoteTarkistinValinta(int maxSyote, int minSyote)
{
    bool virhe; //Boolian, jota k‰ytet‰‰n virheen ilmoittamisessa
    int syote; //Luku, mik‰ syˆtet‰‰n


    //Do, joka tarkistaa syˆtteen siihen asti, ett‰ virhe on ep‰tosi
    do
    {
        //Laitetaan alussa virhe ep‰todeksi, koska oletetaan, ett‰ kaikki on hyvin
        virhe = false;


        //Pyyd‰‰n syˆtett‰ ja otetaan se sis‰‰n
        cout << endl << "Syˆt‰ luku: ";
        cin >> syote;
        cout << endl;


        //Jos syˆtteen antaminen ei onnistu, niin tehd‰‰n n‰in
        if (cin.fail())
        {
            //Tulostetaan virheviesti ja pyydet‰‰n syˆtt‰m‰‰n luku uudestaan
            cout << "#############################" << endl;
            cout << "Virhe. Syˆt‰ vain lukuarvoja!" << endl;
            cout << "#############################" << endl << endl;

            cout << "Tee syˆte uudelleen! (Syˆta numero " << minSyote << "-" << maxSyote << ")" << endl;


            //Laitetaan virhe todeksi, koska virhe on tapahtunut
            virhe = true;


            //Siivotaan muuttuja
            cin.clear();
            cin.ignore(80, '\n');
        }
        //Jos syˆte on yli tai ali hyv‰ksytt‰v‰n arvon, niin tehd‰‰n n‰in
        else if (syote > maxSyote || syote < minSyote)
        {
            //Tulostetaan virheviesti ja pyydet‰‰n syˆtt‰m‰‰n luku uudestaan
            cout << endl;
            cout << "#####################################" << endl;
            cout << "Virhe. Syˆt‰ vain numero v‰lilt‰ " << minSyote << "-" << maxSyote << "!" << endl;
            cout << "#####################################" << endl << endl;

            cout << "Tee syˆte uudelleen! (Syˆta numero " << minSyote << "-" << maxSyote << ")" << endl;


            //Laitetaan virhe todeksi, koska virhe on tapahtunut
            virhe = true;


            //Siivotaan muuttuja
            cin.clear();
            cin.ignore(80, '\n');
        }

    } while (virhe);

    //Jos kaikki oli hyvin, niin palautetaan lopussa syˆte
    return syote;
}


/*
Aliohjelma, joka tarkistaa stringin syˆtteen. Rivill‰ 1057
-muuttujaa k‰ytet‰‰n aliohjelman kysymykseen 
esim. jos laittaa muuttujaksi "etunimesi", niin aliohjelma sanoo "Syˆt‰ etunimesi"
*/
string SyoteTarkistinString(string syoteTeksti)
{
    bool virhe; //Boolian, jota k‰ytet‰‰n virheen ilmoittamisessa
    string syote; //Teksti, mik‰ syˆtet‰‰n


    //Do, joka tarkistaa syˆtteen siihen asti, ett‰ virhe on ep‰tosi
    do
    {
        //Laitetaan alussa virhe ep‰todeksi, koska oletetaan, ett‰ kaikki on hyvin
        virhe = false;


        //Pyyd‰‰n syˆtett‰ ja otetaan se sis‰‰n
        cout << endl << "Syˆt‰ " << syoteTeksti << ": ";
        cin >> syote;
        cout << endl;

        
        //Jos syˆtteen antaminen ei onnistu, niin tehd‰‰n n‰in
        if (cin.fail())
        {
            //Tulostetaan virheviesti ja pyydet‰‰n syˆtt‰m‰‰n syˆte uudestaan
            cout << "#############################" << endl;
            cout << "Syˆtt‰misess‰ tapahtui virhe!" << endl;
            cout << "#############################" << endl << endl;

            cout << "Tee syˆte uudelleen!" << endl;


            //Laitetaan virhe todeksi, koska virhe on tapahtunut
            virhe = true;


            //Siivotaan muuttuja
            cin.clear();
            cin.ignore(80, '\n');
        }

    } while (virhe);

    //Jos kaikki oli hyvin, niin palautetaan lopussa syˆte
    return syote;
}