#ifndef SLEEPCHRONOGRAPH_H
#define SLEEPCHRONOGRAPH_H

#include <iostream>             // ios_base::failure
#include <thread>
#include <mutex>
#include <condition_variable>

namespace nsUtil
{
    // L'injection de la NTCTS
    //
    //====================//
    //                    //
    extern std::string clrscr; //
    //                    //
    //====================//
    //
    // vide l'ecran et positionne le curseur en haut a gauche

    // La
    //
    //============//
    //            //
    class gotoxy; //
    //            //
    //============//
    //
    // est un injecteur qui positionne le curseur a l'ecran
    // en position x et y :
    //
    // cout << gotoxy (10, 20) << ...

    class gotoxy
    {
      public :
        gotoxy (int x, int y);
        friend std::ostream & operator << (std::ostream & os,
                                           const gotoxy & m);
      private :
        int m_x, m_y;

    }; // gotoxy()

    std::ostream & operator << (std::ostream & os, const gotoxy & m);

    // La fonction
    //
    //===============================================//
    //                                               //
    void SleepChronogram (                           //
                          unsigned Num,              //
                          unsigned & Col,            //
                          char Status,               //
                          unsigned Duree,            //
                          std::mutex & Mutex,      //
                          unsigned Decal   = 10u,    //
                          unsigned Periode = 1u      //
                         );              //
    //                                               //
    //===============================================//
    //
    // fait dormir le thread qui l'appelle pendant Duree secondes au
    // moyen de la fonction ::sleep().
    //
    // Elle est destinee a tracer des chronogrammes de threads pendant
    // leur periode de sommeil, chronogrammes qui apparaissent comme
    // dans l'exemple ci-dessous :
    //
    // ------------ Haut de l'ecran ----------------------
    //                                  ^
    //                                  |
    //                                  |
    //                                Decal
    //                                  |
    //                                  |
    //                                  v
    // AAADDDDD
    // AAAADDDDD
    // AAAAADDX
    //
    //------------- Bas de l'ecran -----------------------
    //
    // Status  : caractere affiche en exclusion mutuelle.
    //           Il est sense representer l'etat courant du thread
    // Mutex   : mutex POSIX utilise pour l'affichage
    // Periode : periodicite de l'affichage (en secondes)
    // Duree   : duree totale de l'affichage (= du sommeil du thread)
    // Decal   : nombre de lignes vierges en haut de l'ecran
    // Num     : numero de la ligne d'affichage sur laquelle est affiche
    //           le caractere Status.
    //           Le curseur apparait donc a la ligne (Decal + Num).
    //           Num est en general le numero du thread (e partir de 0).
    // Col     : en entree : colonne du premier affichage du caractere
    //           Status
    //         : en sortie : position du curseur en fin d'affichage
    //
    // La fonction Sleep () est susceptible de lever deux exceptions
    // - std::ios_base::failure lors de l'injection du caractere
    // - nsUtil::CExcFct lors du (de)-verrouillage du mutex
    //   (voir CException.h)

} // namespace nsUtil

#endif // SLEEPCHRONOGRAPH_H
