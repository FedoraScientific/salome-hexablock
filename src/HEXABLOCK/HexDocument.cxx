
// C++ : La clase principale de Hexa

#include "HexDocument.hxx"

#include "HexEltBase.hxx"
#include "HexVertex.hxx"
#include "HexEdge.hxx"
#include "HexQuad.hxx"
#include "HexHexa.hxx"

#include "HexElements.hxx"
#include "HexCrossElements.hxx"

#include "HexVector.hxx"
#include "HexCylinder.hxx"
#include "HexPipe.hxx"
#include "HexMatrix.hxx"
#include "HexCloner.hxx"
#include "HexPropagation.hxx"
#include "HexLaw.hxx"

#include "HexXmlWriter.hxx"
#include "HexXmlTree.hxx"
#include "HexGlobale.hxx"
#include "HexGroups.hxx"

BEGIN_NAMESPACE_HEXA

// ======================================================== Constructeur
Document::Document (cpchar filename)
{
   glob = Globale::getInstance ();

   doc_db        = 0;
   nbr_errors    = 0;
   doc_modified  = true;
   doc_name      = filename;
   doc_tolerance = 1e-6;
   nbr_laws      = 0;
   nbr_propagations = 0;
   maj_propagation  = true;

   addLaw ("DefaultLaw", 0);

   for (EnumElt type = EL_NONE ; type < EL_MAXI ; type=(EnumElt) (type+1))
       {
       doc_first_elt [type] = doc_last_elt [type] = new EltBase (); 
       doc_nbr_elt   [type] = 0;
       }
}
// ======================================================== Destructeur
Document::~Document ()
{
   for (EnumElt type = EL_NONE ; type < EL_MAXI ; type=(EnumElt) (type+1))
       {
       //  printf ("____________________________ Type=%d\n", type);
       EltBase* next = NULL;
       for (EltBase* elt=doc_first_elt [type] ; elt != NULL ; elt=next) 
           {
           //  elt->dump();
           next = elt->next();
           //  Display(elt);e0057
           //  if (type!=EL_NONE) 
              delete elt;
           }
       }
}
// ======================================================== addVector
Vector* Document::addVector (double dx, double dy, double dz)
{
   Vector* pv = new Vector (this, dx, dy, dz); 
   doc_vector.push_back (pv);
   return  pv;
}
// ======================================================== addVectorVertices
Vector* Document::addVectorVertices (Vertex* va, Vertex* vb)
{
   Vector* pv = addVector (vb->getX () - va->getX (),
                           vb->getY () - va->getY (),
                           vb->getZ () - va->getZ ());
   return  pv;
}
// ======================================================== addVertex
Vertex* Document::addVertex (double x, double y, double z)
{
   Vertex* pv   = new Vertex (this, x, y, z);
   doc_modified = true;
   return pv;
}
// ======================================================== addEdge
Edge* Document::addEdge (Vertex* va, Vertex* vb)
{
   if (va==NULL || vb==NULL) 
      return NULL;

   Edge* arete  = new Edge (va, vb);
   doc_modified = true;
   return arete;
}
// ======================================================== addEdge (bis)
Edge* Document::addEdge (Vertex* va, Vector* vec)
{
   doc_modified = true;
   Vertex* vb   = addVertex (va->getX () + vec->getDx(), 
                             va->getY () + vec->getDy(), 
                             va->getZ () + vec->getDz());

   Edge*  arete = addEdge (va, vb);
   return arete;
}
// ======================================================== addQuadVertices
Quad* Document::addQuadVertices (Vertex* va, Vertex* vb, Vertex* vc, Vertex* vd)
{
   Quad* face   = new Quad (va, vb, vc, vd);
   doc_modified = true;
   return face;
}
// ======================================================== addQuad
Quad* Document::addQuad (Edge* ea, Edge* eb, Edge* ec, Edge* ed)
{
   Quad* face   = new Quad (ea, eb, ec, ed);
   doc_modified = true;
   return face;
}
// ======================================================== addHexaVertices
Hexa* Document::addHexaVertices (Vertex* va, Vertex* vb, Vertex* vc, Vertex* vd,
                                 Vertex* ve, Vertex* vf, Vertex* vg, Vertex* vh)
{
   Hexa* pave   = new Hexa (va, vb, vc, vd, ve, vf, vg, vh);
   doc_modified = true;
   return pave;
}
// ======================================================== addHexa
Hexa* Document::addHexa (Quad* qa, Quad* qb, Quad* qc, Quad* qd, Quad* qe, 
                         Quad* qf)
{
   Hexa* pave   = new Hexa (qa, qb, qc, qd, qe, qf);
   doc_modified = true;
   return pave;
}
// ======================================================== addCylinder
Cylinder* Document::addCylinder (Vertex* b, Vector* d, double r,  double h)
{
   doc_modified  = true;
   Cylinder* cyl = new  Cylinder (b, d, r, h);
   doc_cylinder.push_back (cyl);
   return    cyl;
}
// ======================================================== addPipe
Pipe* Document::addPipe (Vertex* b, Vector* d, double ri, double re, double h)
{
   doc_modified = true;
   Pipe*  tuyau = new  Pipe (b, d, ri, re, h);
   doc_pipe.push_back (tuyau);
   return tuyau;
}
// ======================================================== makeCartesian
Elements* Document::makeCartesian (Vertex* v, Vector* dir, 
                            int px, int py, int pz, int mx, int my, int mz)
{
   Vector* v1 = new Vector (this, dir->getDx(), 0, 0); 
   Vector* v2 = new Vector (this, 0, dir->getDy(), 0); 
   Vector* v3 = new Vector (this, 0,0,  dir->getDz()); 
   Elements*  grille = new Elements (this);
   grille->makeCartesianGrid (v, v1, v2, v3, px, py, pz, mx, my, mz);
   return grille;
}
// ======================================================== makeCartesian
Elements* Document::makeCartesian (Vertex* v, Vector* v1, Vector* v2, 
                    Vector* v3, int px, int py, int pz, int mx, int my, int mz)
{
   Elements* grille = new Elements (this);
   grille->makeCartesianGrid (v, v1, v2, v3, px, py, pz, mx, my, mz);
   return grille;
}
// ======================================================== makeSpherical
Elements* Document::makeSpherical (Vertex* c, Vector* dv, int nb, double k)
{
   Elements* grille = new Elements (this);
   grille->makeSphericalGrid (c, dv, nb, k);
   return grille;
}
// ======================================================== makeCylindrical
Elements* Document::makeCylindrical (Vertex* c, Vector* b, Vector* h, 
        double dr, double da, double dl, int nr, int na, int nl, bool fill)
{
   Elements* grille = new Elements (this);
   grille->makeCylindricalGrid (c, b, h, dr, da, dl, nr, na, nl, fill);
   return grille;
}
// ======================================================== findVertex
Vertex* Document::findVertex (double vx, double vy, double vz)
{
   double xmin = vx - doc_tolerance;
   double xmax = vx + doc_tolerance;
   double ymin = vy - doc_tolerance;
   double ymax = vy + doc_tolerance;
   double zmin = vz - doc_tolerance;
   double zmax = vz + doc_tolerance;

   for (EltBase* elt = doc_first_elt[EL_VERTEX]->next (); elt!=NULL;
                 elt = elt->next())
       {
       if (elt->isHere())
          { 
          Vertex* node = static_cast <Vertex*> (elt); 
          if (node->isin (xmin, xmax, ymin, ymax, zmin, zmax))
             return node;
          }
       }
   return NULL;
}
// ======================================================== findEdge
Edge* Document::findEdge (Vertex* v1, Vertex* v2)
{
   for (EltBase* elt = doc_first_elt[EL_EDGE]->next (); elt!=NULL;
                 elt = elt->next())
       {
       Edge* candidat  = static_cast <Edge*> (elt); 
       if (candidat->definedBy (v1, v2))
          return candidat;
       }
   return NULL;
}
// ======================================================== findQuad
Quad* Document::findQuad (Vertex* v1, Vertex* v2)
{
   for (EltBase* elt = doc_first_elt[EL_QUAD]->next (); elt!=NULL;
                 elt = elt->next())
       {
       Quad* candidat  = static_cast <Quad*> (elt); 
       if (candidat->definedBy (v1, v2))
          return candidat;
       }
   return NULL;
}
// ======================================================== findHexa
Hexa* Document::findHexa (Vertex* v1, Vertex* v2)
{
   for (EltBase* elt = doc_first_elt[EL_HEXA]->next (); elt!=NULL;
                 elt = elt->next())
       {
       Hexa* candidat  = static_cast <Hexa*> (elt); 
       if (candidat->definedBy (v1, v2))
          return candidat;
       }
   return NULL;
}
// ======================================================== makeCylinder
int index_tv (Vertex* table[], Vertex* elt)
{
   for (int nro=0; nro<QUAD4; nro++)
        if (elt == table[nro])
           return nro;

   return NOTHING;
}
// ======================================================== mergeQuads
int Document::mergeQuads (Quad* par, Quad* old, Vertex* v1, Vertex* v2, 
                                                Vertex* v3, Vertex* v4)
{
   update ();

   if (   par==NULL || par->isDeleted() || old==NULL || old->isDeleted() 
       || v1 ==NULL || v1 ->isDeleted() || v2 ==NULL || v2 ->isDeleted() 
       || v3 ==NULL || v3 ->isDeleted() || v4 ==NULL || v4 ->isDeleted()) 
      return HERR;

   for (int nro=0 ; nro<QUAD4 ; nro++) 
       if (old->indexVertex (par->getVertex(nro))!= NOTHING)
          return HERR+nro+1;

   if (debug())
      {
      printf ("  ----------------- mergeQuads : \n");
      HexDump (old);
      HexDump (par);
      HexDump (v1);
      HexDump (v2);
      HexDump (v3);
      HexDump (v4);
      }

   Vertex *tv1 [QUAD4], *tv2 [QUAD4];
   Edge   *te1 [QUAD4], *te2 [QUAD4];

   int ier1 = par->ordoVertex (v1, v3, tv1);
   int ier2 = old->ordoVertex (v2, v4, tv2);
   if (ier1 != HOK)      return ier1;
   else if (ier2 != HOK) return ier2;

   for (int nro=0 ; nro<QUAD4 ; nro++)
       {
       te1 [nro] = par->getEdge(nro);
       Vertex* va1 = te1[nro]->getVertex(V_AMONT);
       Vertex* vb1 = te1[nro]->getVertex(V_AVAL);
       int na = index_tv  (tv1, va1);
       int nb = index_tv  (tv1, vb1);
       if (na==NOTHING || nb==NOTHING)
          return HERR;

       te2 [nro] = old->findEdge (tv2[na], tv2[nb]);
       if (te2[nro]==NULL)
          return HERR;
       }

   if (debug())
      {
      printf ("  ----------------- Correspondances mergeQuads : \n");
      for (int nro=0 ; nro<QUAD4 ; nro++)
          {
          printf ("  %d  : ", nro);
          tv2 [nro]->printName(" -> ");
          tv1 [nro]->printName("\n");
          }
      for (int nro=0 ; nro<QUAD4 ; nro++)
          {
          printf ("  %d  : ", nro);
          te2 [nro]->printName(" (");
          te2 [nro]->getVertex(0)->printName(", ");
          te2 [nro]->getVertex(1)->printName(") -> ");
          te1 [nro]->printName(" (");
          te1 [nro]->getVertex(0)->printName(", ");
          te1 [nro]->getVertex(1)->printName(")\n");
          }
      }

   replaceQuad (old, par);
   for (int nro=0 ; nro<QUAD4 ; nro++) 
       replaceEdge   (te2[nro], te1[nro]);
   for (int nro=0 ; nro<QUAD4 ; nro++) 
       replaceVertex (tv2[nro], tv1[nro]);

   maj_connection = false;
   purge_elements = false;
   return HOK;
}
// ======================================================== mergeEdges
int Document::mergeEdges (Edge* e1, Edge* e2, Vertex* v1, Vertex* v2)
{
   if (e1==NULL || e1->isDeleted() || e2==NULL || e2->isDeleted()) 
      return HERR;

   for (int nro=0 ; nro<V_TWO ; nro++) 
       if (e1->index (e2->getVertex(nro))!= NOTHING)
          return HERR+nro+1;

   Vertex *tv1 [V_TWO], *tv2 [V_TWO];

   int ier1 = e1->anaMerge (v1, tv1);
   int ier2 = e2->anaMerge (v2, tv2);

   if (ier1 != HOK)      return ier1;
   else if (ier2 != HOK) return ier2;

   replaceEdge (e2, e1);
   for (int nro=0 ; nro<V_TWO ; nro++) 
       replaceVertex (tv2[nro], tv1[nro]);

   maj_connection = false;
   return HOK;
}
// ======================================================== mergeVertices
int Document::mergeVertices (Vertex* v1, Vertex* v2)
{
   if (v1==v2 || v1==NULL || v1->isDeleted() 
              || v2==NULL || v2->isDeleted())
      return HERR;

   replaceVertex (v2, v1);

   maj_connection = false;
   return HOK;
}
// ======================================================== replaceVertex
void Document::replaceVertex (Vertex* old, Vertex* par)
{
   if (old==par)
      return;

   for (int type=EL_EDGE ; type <= EL_HEXA ; type++)
       {
       for (EltBase* elt = doc_first_elt[type]->next (); elt!=NULL;
                     elt = elt->next())
           if (elt->isHere ())
               elt->replaceVertex (old, par);
       }
   old->suppress ();
}
// ======================================================== replaceEdge
void Document::replaceEdge (Edge* old, Edge* par)
{
   if (old==par)
      return;

   for (int type=EL_QUAD ; type <= EL_HEXA ; type++)
       {
       for (EltBase* elt = doc_first_elt[type]->next (); elt!=NULL;
                     elt = elt->next())
           if (elt->isHere ())
               elt->replaceEdge (old, par);
       }
   old->suppress ();
}
// ======================================================== replaceQuad
void Document::replaceQuad (Quad* old, Quad* par)
{
   if (old==par)
      return;

   for (EltBase* elt = doc_first_elt[EL_HEXA]->next (); elt!=NULL;
                 elt = elt->next())
       if (elt->isHere ())
          {
          Hexa* cell = static_cast <Hexa*> (elt);
          cell->replaceQuad (old, par);
          }
   old->suppress ();
}
// ======================================================== prismQuad
Elements* Document::prismQuad  (Quad* start, Vector* dir, int nb)
{
   Quads  tstart;
   tstart.push_back (start);

   update ();
   Elements* prisme = prismQuads (tstart, dir, nb);
   return    prisme;
}
// ======================================================== prismQuads
Elements* Document::prismQuads (Quads& tstart, Vector* dir, int nb)
{
   if (nb<=0) return NULL;

   Elements*  prisme = new Elements (this);
   prisme->prismQuads (tstart, dir, nb);
   return prisme;
}
// ======================================================== joinQuads
Elements* Document::joinQuads (Quads& start, Quad* dest, Vertex* v1, 
		               Vertex* v2, Vertex* v3, Vertex* v4, int nb)
{
   if (nb<=0)      return NULL;

   update ();
   Elements*  joint = new Elements (this);
   joint->joinQuads (start, nb, v1, v2, v3, v4, dest);
   return joint;
}
// ======================================================== joinQuad
Elements* Document::joinQuad (Quad* start, Quad* dest, Vertex* v1, 
                              Vertex* v2,  Vertex* v3, Vertex* v4, int nb)
{
   Quads  tstart;
   tstart.push_back (start);

   Elements* joint = joinQuads (tstart, dest, v1, v2, v3, v4, nb);
   return    joint;
}
// ========================================================== getHexa
Hexa* Document::getHexa (int nro) 
{
   return static_cast <Hexa*> (getElement (EL_HEXA, nro));
}
// ========================================================== getQuad
Quad* Document::getQuad (int nro) 
{
   return static_cast <Quad*> (getElement (EL_QUAD, nro));
}
// ========================================================== getEdge
Edge* Document::getEdge (int nro) 
{ 
   return static_cast <Edge*> (getElement (EL_EDGE, nro));
}
// ========================================================== getVertex
Vertex* Document::getVertex (int nro) 
{ 
   return static_cast <Vertex*> (getElement (EL_VERTEX, nro)); 
}
// ========================================================== countElement
int Document::countElement (EnumElt type)
{ 
   int compteur = 0;
   for (EltBase* elt = doc_first_elt[type]->next (); elt!=NULL;
                 elt = elt->next())
       if (elt->isHere())
          compteur ++;

   return compteur;
}
// ========================================================== getElement
EltBase* Document::getElement (EnumElt type, int nro) 
{ 
   int compteur = 0;
   for (EltBase* elt = doc_first_elt[type]->next (); elt!=NULL;
                 elt = elt->next())
       {
       if (elt->isHere())
          {
          if (compteur>=nro)
             return elt;
          compteur ++;
          }
       }
   return NULL;
}
// ========================================================= addLaw
Law* Document::addLaw (const char* name, int nbnodes)
{ 
   Law* loi = new Law (name, nbnodes);
   doc_laws.push_back (loi);
   nbr_laws ++;
   return loi;
}
// ========================================================= GetLaw
Law* Document::getLaw (int nro)
{ 
   if (nro <0 || nro>= nbr_laws)
      return NULL;

   return doc_laws [nro];
}
// ========================================================= FindLaw
Law* Document::findLaw (const char* name)
{ 
   std::string nom = name;
   for (int nro=0 ; nro<nbr_laws; nro++)
       if (doc_laws [nro]->getName() == nom)
          return doc_laws [nro];

   return NULL;
}
// ========================================================= removeLaw
int Document::removeLaw (Law* loi)
{ 
   for (int nro=1 ; nro<nbr_laws; nro++)
       if (doc_laws [nro] == loi)
          {
          delete doc_laws [nro];
          doc_laws.erase (doc_laws.begin()+nro);
          nbr_laws= doc_laws.size();
          return HOK;
	  }

   return HERR;
}
// ========================================================= majPropagation
void Document::majPropagation ()
{ 
   majReferences ();
   if (purge_elements)
       purge ();

   for (int nro=0 ; nro<nbr_propagations ; nro++)
       {
       delete doc_propagation [nro];
       }

   doc_propagation.clear ();
   nbr_propagations = 0;
   maj_propagation  = false;

   for (EltBase* elt = doc_first_elt[EL_EDGE]->next (); elt!=NULL;
                 elt = elt->next())
       {
       Edge* arete = static_cast <Edge*> (elt);
       if (arete!=NULL)
           arete->setPropag (NOTHING, true);
       }

   //  markAll (NO_COUNTED, EL_EDGE);

   for (EltBase* elt = doc_first_elt[EL_HEXA]->next (); elt!=NULL;
                 elt = elt->next())
       {
       Hexa* cell = static_cast <Hexa*> (elt);
       if (cell!=NULL && cell->isHere())
          {
          for (int ne=0 ; ne<HE_MAXI ; ne++)
              {
              Edge* arete = cell->getEdge(ne);
              if (arete->getPropag()<0)
                 {
                 Propagation* prop = new Propagation (); 
                 doc_propagation.push_back (prop);
                 arete->propager (prop, nbr_propagations);
                 nbr_propagations ++;
		 prop->majLaw();
                 }
              }
          }
       }
}
// ======================================================== countPropagation
int Document::countPropagation ()
{
   if (maj_propagation)
       majPropagation ();

   return nbr_propagations;
}
// ======================================================== getPropagation
Propagation* Document::getPropagation (int nro)
{
   if (maj_propagation)
       majPropagation ();

   if (nro < 0 || nro >= nbr_propagations)
       return NULL;

   return doc_propagation [nro];
}
// ======================================================== findPropagation
Propagation* Document::findPropagation (Edge* arete)
{
   if (arete==NULL) 
       return NULL;

   if (maj_propagation)
       majPropagation ();

   return getPropagation (arete->getPropag ());
}
// ======================================================== disconnectQuad
Elements* Document::disconnectQuad (Hexa* cell, Quad* element)
{
   if (cell==NULL || element==NULL) 
      return NULL;

   update ();
   Elements* crees = cell->disconnectQuad (element);

   if (crees!=NULL) 
       majReferences ();

   return crees;
}
// ======================================================== disconnectEdge
Elements* Document::disconnectEdge (Hexa* cell, Edge* element)
{
   if (cell==NULL || element==NULL) 
      return NULL;

   update ();
   Elements* crees = cell->disconnectEdge (element);

   if (crees!=NULL)
       majReferences ();
   return crees;
}
// ======================================================== disconnectVertex
Elements* Document::disconnectVertex (Hexa* cell, Vertex* element)
{
   if (cell==NULL || element==NULL) 
      return NULL;

   update ();
   Elements* crees = cell->disconnectVertex (element);

   if (crees!=NULL) 
       majReferences ();
   return crees;
}
// ======================================================== cut
Elements* Document::cut (Edge* edge, int nbcuts)
{
   Elements* t_hexas = new Elements (this);

   if (edge==NULL || nbcuts<=0)
      return t_hexas;

   Propagation* prop    = findPropagation (edge);
   const Edges& t_edges = prop->getEdges ();

   t_hexas->cutHexas (t_edges, nbcuts);

   majPropagation ();
   return t_hexas;
}
// ======================================================== addGroup
Group* Document::addGroup    (cpchar name, EnumGroup kind)
{
   Group* grp = new Group (name, kind);
   doc_group.push_back (grp);
   return grp;
}
// ======================================================== findGroup
Group* Document::findGroup   (cpchar name)
{
   int nbre = doc_group.size();

   for (int ng=0 ; ng<nbre ; ng++) 
       if (Cestegal (doc_group [ng]->getName(), name))
          return doc_group [ng];

   return NULL;
}
// ======================================================== removeGroup
int Document::removeGroup (Group* grp)
{
   int nbre = doc_group.size();
   for (int ng=0 ; ng<nbre ; ng++) 
       {
       if (grp == doc_group [ng])
          {
          doc_group.erase (doc_group.begin() + ng);
          delete grp;
          return HOK;
          }
       }
                      // Pas trouve dans la liste. On detruit quand meme
   delete grp;
   return HERR;
}// ======================================================== makeCylinder
Elements* Document::makeCylinder (Cylinder* cyl, Vector* base, int nr, int na, 
                                                                       int nl)
{
   Elements* grille = new Elements (this);
   grille->makeCylinder (cyl, base, nr, na, nr);
   return grille;
}
// ======================================================== makeCylinders
CrossElements* Document::makeCylinders (Cylinder* cyl1, Cylinder* cyl2)
{
   CrossElements* grille = new CrossElements (this);
   grille->crossCylinders (cyl1, cyl2, true);
   return grille;
}

// ======================================================== makePipe
Elements* Document::makePipe (Pipe* pipe, Vector* bx, int nr, int na, int nl)
{
   Elements* grille = new Elements (this);
   grille->makePipe (pipe, bx, nr, na, nr);
   return grille;
}
// ======================================================== makePipes
CrossElements* Document::makePipes (Pipe* pipe1, Pipe* pipe2)
{
   CrossElements* grille = new CrossElements (this);
   grille->crossCylinders (pipe1, pipe2, false);
   return grille;
}
END_NAMESPACE_HEXA
