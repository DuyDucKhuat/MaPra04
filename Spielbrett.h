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

class Spielbrett{
    private:
    std::vector<int>  Brett;
    std::vector<int> Stand;
    size_t cols, rows;
    
    
    public:
    explicit Spielbrett (size_t cols, size_t rows);
    ~Spielbrett() {};
    int& operator() (size_t col, size_t r);
    void zug(size_t col, size_t spieler);
    bool checkLine(int type, size_t c, size_t r);
    
};

#endif /* Spielbrett_h */
