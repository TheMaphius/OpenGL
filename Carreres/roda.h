#ifndef RODA_H
#define RODA_H

#include <objecte.h>

class Roda : public Objecte
{
public:
    Roda();
    Material* getMaterial();
    Llum* getLlum();
    void doGouraud();
};

#endif // RODA_H
