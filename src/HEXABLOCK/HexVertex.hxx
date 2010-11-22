
// Class : Gestion des sommets 
//
#ifndef __VERTEX_H_
#define __VERTEX_H_

#include "HexEltBase.hxx"

BEGIN_NAMESPACE_HEXA

class Vertex : public EltBase 
{
public :
   double getX()   { return v_x; }
   double getY()   { return v_y; }
   double getZ()   { return v_z; }

   void setX (double v)   { v_x = v ; }
   void setY (double v)   { v_y = v ; }
   void setZ (double v)   { v_z = v ; }

public :
   Vertex (Document* prev, double x=0.0, double y=0.0, double z=0.0); 
   Vertex (Vertex* other);
   virtual ~Vertex () {}
   virtual void dump () ;
   virtual void saveXml (XmlWriter& xml);

   void   setScalar (double val)             { v_scalar = val ; }
   double getScalar ()                       { return v_scalar ; }

   void setCoord  (double x, double y, double z);
   bool isin      (double xmin, double xmax, double ymin, double ymax, 
                                            double zmin, double zmax);
   Edge* getParent (int nro); 
   Vertex* makeSymetric (Vertex* other);
   void    translate  (Vector* vecteur, double fact=1.0);

   static Vertex* createMiddle (Vertex* left, Vertex* right);

private :
    double v_x;
    double v_y;
    double v_z;

    double v_scalar;
};
// ========================================================= Constructeur bis 
inline Vertex::Vertex (Vertex* other)
      : EltBase (other->dad(), EL_VERTEX)
{
   v_x = other->v_x;
   v_y = other->v_y;
   v_z = other->v_z;
   v_scalar = 0;
}
// ========================================================= dump 
inline void Vertex::dump ()
{
   printName (" = ");
   if (NOT isHere ())
      {
      printf ("(*** deleted ***)\n");
      return;
      }

   printf ("(%g, %g, %g)", v_x,v_y,v_z);
   dumpRef ();
}
// ========================================================= setCoord 
inline void Vertex::setCoord (double x, double y, double z)
{
   v_x = x;
   v_y = y;
   v_z = z;
}
// ========================================================= isin 
inline bool Vertex::isin  (double xmin, double xmax, double ymin, double ymax, 
                                             double zmin, double zmax)
{
   bool   rep =   v_x >= xmin && v_x <= xmax
               && v_y >= ymin && v_y <= ymax
               && v_z >= zmin && v_z <= zmax;
   return rep;
}
END_NAMESPACE_HEXA
#endif
