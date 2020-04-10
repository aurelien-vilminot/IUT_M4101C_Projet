/**
 * @file   main.cpp
 *
 * @author Aurélien VILMINOT
 *
 * @date   10/04/2020
 *
 * @brief  Programmation Concurrente
 *
 * Implementation du probleme de distribiteur de billets adapte du "TheLittleBookOfSemaphore" (The BarberShop Problem)
 *
 */
#include <vector>
#include "SleepChronogram.h"

using namespace std;
using namespace nsUtil;

namespace  {
    enum { KExcArg  = 253 };    // Erreur d'arguments de main()
    condition_variable condition;
    mutex mtxCondVar, ioMtx;

    unsigned nbPlacesHall;
    bool personnesRetirant = false;

    bool arriverHallBanque(void) {
        unique_lock<mutex> lock(mtxCondVar);
        if (nbPlacesHall == 0) {
            return false;
        } else {
            --nbPlacesHall;
            return true;
        }
    }

    void allerRetirer(void) {
        unique_lock<mutex> lock(mtxCondVar);
        while (personnesRetirant) {
            condition.wait(lock);
        }
        personnesRetirant = true;
        ++nbPlacesHall;

        condition.notify_all();
    }

    void finRetrait(void) {
        unique_lock<mutex> lock(mtxCondVar);
        personnesRetirant = false;
        condition.notify_all();
    }

    class Personne {
        static unsigned s_ID;

        unsigned myID;
        unsigned myDureeArrivee;

        public:
            Personne(unsigned dureeArrivee): myID(++s_ID), myDureeArrivee(dureeArrivee){}

            void operator () (void) {
                unsigned col = 1;
                unsigned dureeRetrait = 5;
                SleepChronogram(myID, col, 'A', myDureeArrivee, ioMtx);
                if (!arriverHallBanque()) {
                    SleepChronogram(myID, col, 'E', 1, ioMtx);
                    return;
                }
                SleepChronogram(myID, col, 'W', 1, ioMtx);
                allerRetirer();
                SleepChronogram(myID, col, 'R', dureeRetrait, ioMtx);
                finRetrait();
                SleepChronogram(myID, col, 'F', 1, ioMtx);
            } //operator()
    }; //Personne
} // namespace

unsigned Personne::s_ID = 0;

int main(int argc, char * argv []) {
    if (argc != 1) {
         cerr << "Usage : DistributeurThreadsBoost\n";
         return KExcArg;
    }

    cout << "Le distributeur de billets sort de veille." << endl;

    unsigned nbThreads;
    while (true) {
        cout << "Saisir le nombre de clients (threads) : ";
        cin >> nbThreads;
        cout << "Saisir le nombre de personnes maximun dans la banque : ";
        cin >> nbPlacesHall;
        if (nbThreads != 0 || nbPlacesHall != 0) {
            break;
        } else {
            cout << clrscr << flush
                 << "Merci de saisir des chiffres supérieurs à 0 !" << endl;
        }
    }

    vector <Personne> vPersonnes;
    vPersonnes.reserve (nbThreads);
    for (unsigned dureeArrivee, i = 0; i < nbThreads; ++i) {
        while (true) {
            cout << "Personne " << i + 1 << " : ";
            cout << endl << "Décaler l'arrivée de combien de secondes ? : ";
            cin >> dureeArrivee;
            if (dureeArrivee != 0) {
                break;
            } else {
                cout << "Merci de saisir un chiffre supérieur à 0 !" << endl;
            }
        }

        vPersonnes.push_back(Personne(dureeArrivee));
    }

    cout << clrscr << flush;

    vector <thread *> vThreads;
    for (unsigned i = nbThreads; i--; ) {
        vThreads.push_back(new thread (vPersonnes[i]));
    }
    for (unsigned i = nbThreads; i--; ) {
        vThreads [i]->join ();
    }

    cout << gotoxy (0, nbThreads + 12);
    cout << "Le distributeur de billets se met en veille." << endl;
    return 0;
}
