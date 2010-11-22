
// C++ : Gestion des aretes

#include "HexEdge.hxx"
#include "HexVertex.hxx"
#include "HexQuad.hxx"

#include "HexPropagation.hxx"
#include "HexXmlWriter.hxx"
#include "HexShape.hxx"

static int niveau = 0;

BEGIN_NAMESPACE_HEXA

// ======================================================== Constructeur
Edge::Edge (Vertex* va, Vertex* vb)
    : EltBase (va->dad(), EL_EDGE)
{
   e_vertex [V_AMONT] = va; 
   e_vertex [V_AVAL ] = vb;

   e_propag = NOTHING;
   e_way    = true;
   e_law    = NULL;

   majReferences ();
}
// ======================================================== Constructeur 2
Edge::Edge (Edge* other)
    : EltBase (other->dad(), EL_EDGE)
{
   e_vertex [V_AMONT] = e_vertex [V_AVAL ] = NULL;

   e_propag = NOTHING;
   e_way    = true;
   e_law    = NULL;
}
// ======================================================== majReferences
void Edge::majReferences ()
{
   e_vertex [V_AMONT]->addParent (this);
   e_vertex [V_AVAL ]->addParent (this);
}
// ======================================================== anaMerge
int Edge::anaMerge (Vertex* orig, Vertex* tv1[])
{
   tv1 [0] = orig;

   if (orig == e_vertex [V_AMONT])
      tv1 [1] = e_vertex[V_AVAL];
   else if (orig == e_vertex [V_AVAL])
      tv1 [1] = e_vertex[V_AMONT];
   else 
      return HERR;

   return HOK;
}
// ========================================================= propager 
void Edge::propager (Propagation* prop, int groupe, int sens)
{
   setPropag (groupe, sens>0);
   prop->addEdge (this);

   niveau ++;
   int nbquads = getNbrParents ();
   for (int nq=0 ; nq<nbquads ; nq++)
       {
       Quad* quad = getParent (nq);
       if (quad!=NULL && quad->hasParents())
          {
          if (debug (1))
             {
             printf ("Prop : niv=%d edge=", niveau);
             printName ();
             printf ("parent=%d quad=", nq);
             quad->printName ("\n");
	     fflush(stdout);
             }

          int meme = 1;
          Edge* arete = quad->getOpposEdge (this, meme);
          if (arete->getPropag () < 0)
              arete->propager (prop, groupe, meme*sens);
          }
       }
   niveau --;
}
// ========================================================= getParent 
Quad* Edge::getParent  (int nro)
{
   return static_cast <Quad*> (getFather (nro));
}
// ========================================================= getParent 
void Edge::saveXml (XmlWriter& xml)
{
   char buffer[12];
   string vertices = e_vertex [V_AMONT]->getName(buffer);
   vertices += " ";
   vertices       += e_vertex [V_AVAL ]->getName(buffer);

   xml.openMark     ("Edge");
   xml.addAttribute ("id",       getName (buffer));
   xml.addAttribute ("vertices", vertices);
   if (el_assoc!=NULL)
      xml.addAttribute ("shape", el_assoc->getBrep ().c_str());
   xml.closeMark ();
}
// ======================================================== replaceVertex
void Edge::replaceVertex (Vertex* old, Vertex* par)
{
   for (int nro=0 ; nro<QUAD4 ; nro++)
       {
       if (e_vertex[nro]==old) 
           {
           e_vertex[nro] = par;
	   if (debug())
	      {
              printf (" Dans ");
              printName ();
              printf (" [%d], ", nro);
              old->printName (" est remplace par ");
              par->printName ("\n");
              }
           }
       }
}
// ======================================================== dump
void Edge::dump ()
{
   printName(" = (");
   if (NOT isHere ())
      {
      printf ("*** deleted ***)\n");
      return;
      }

   e_vertex[V_AMONT]->printName(", ");
   e_vertex[V_AVAL] ->printName(")");
   if (e_propag>=0)
      {
      char signe = e_way ? '+' : '-';
      printf (", Prop= %c%d", signe, e_propag);
      }

   dumpRef ();
}
// ======================================================== dumpPlus
void Edge::dumpPlus ()
{
   dump ();
   for (int nro=0 ; nro < V_TWO ; nro++)
       {
       Vertex* pv = e_vertex[nro];
       printf ( "    ");
       if (pv!=NULL)
          {
          pv->printName ();
          printf (" (%g, %g, %g)\n", pv->getX(),  pv->getY(),  pv->getZ());
          }
       else
          {
          printf (" NULL\n");
          }
       }
}

END_NAMESPACE_HEXA

