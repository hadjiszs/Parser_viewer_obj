#ifndef FLOATVECTOR_H_INCLUDED
#define FLOATVECTOR_H_INCLUDED

class FloatVector
{
    public:

        FloatVector(float px = 0, float py = 0, float pz = 0, float pa = 0);
        FloatVector operator=(FloatVector const& fv);

        float   x,
                y,
                z,
                a;
};

#endif // FLOATVECTOR_H_INCLUDED
