#include <iostream>
#include "FunctorDispather.h"

class Shap
{
public:
    virtual ~Shap() {}
    virtual void Rote(const float) = 0;
};

class Box : public Shap
{
public:
    void Rote(const float angle) override
    {
        std::cout << angle << std::endl;
    }
};

class Rectangle : public Shap
{
    void Rote(const float) override
    {
    }
};

void HandFunctor(Rectangle &b, Box &r)
{
    std::cout << "hand box and rectangle" << std::endl;
}

struct Functor
{
    void operator()(Rectangle &b, Box &r)
    {
        std::cout << "hand Rectange and box in functor" << std::endl;
    }
};


int main(int, char **)
{
    try
    {
        FunctorDispather<Shap> dispather;
        dispather.Add<Rectangle, Box>(std::function<void(Rectangle&,Box&)>(Functor()));
        // dispather.Add<Rectangle, Box>(HandFunctor);
        Box b;
        Rectangle r;
        dispather.Go(b, r);
        std::cout << dispather.Remove<Box, Rectangle>() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
