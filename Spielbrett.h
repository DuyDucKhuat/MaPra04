//
//  Spielbrett.h
//  
//
//  Created by Duc Khuat on 22.05.18.
//

#ifndef Spielbrett_h
#define Spielbrett_h
#include <vector>
#include <iostream>
#include <float.h>
#include <stdlib.h>

class Spielbrett{
    private:
    std::vector<int> Brett;
    std::vector<size_t> Stand; //Stand[col] enthält den Stand (Höhe) der Spalte col.
    size_t cols, rows;

    
    
    
    public:
    explicit Spielbrett (size_t cols, size_t rows);
    ~Spielbrett() {};
    int& operator() (size_t col, size_t r);
    void zug(size_t col, size_t spieler); //setzt den Zug falls möglich, aktualisiert den Stand.
    
    int zähleVert(size_t c, size_t r);
    int zähleHo(size_t c, size_t r);
    int zähleDiagR( size_t c, size_t r);
    int zähleDiagL( size_t c, size_t r);
    bool checkLine(int type, size_t c, size_t r);
    
    double wertung(size_t c);
    size_t nächsterZug();
    size_t maxAlgorithmus(int recursion, size_t& maxI);
    size_t minAlgorithmus(int recursion, size_t& minI);
    
    bool umgebungNull(size_t c);
    template <typename T>
    bool alleGleich (std::vector <T> Werte);
    double feldabschnitt( size_t c, size_t r, int type);



};

#endif /* Spielbrett_h */
