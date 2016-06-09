#ifndef CARROSSERIA_H
#define CARROSSERIA_H

#include <objecte.h>

class Carrosseria : public Objecte
{
public:
    Carrosseria();
    Material* getMaterial();
    Llum* getLlum();
    void doGouraud();
};

#endif // CARROSSERIA_H
