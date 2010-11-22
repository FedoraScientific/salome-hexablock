
// class : Ensembles de propagation

#ifndef __PROPAGATION_H
#define __PROPAGATION_H

#include "HexDocument.hxx"
#include "HexEdge.hxx"
#include "HexXmlWriter.hxx"
#include "HexLaw.hxx"

BEGIN_NAMESPACE_HEXA

class Propagation 
{
public:
   const Edges& getEdges ()        { return prop_edges; }
   Law*  getLaw ();
   bool  getWay ()                 { return prop_way; }

   void  setWay (bool sens)        { prop_way = sens; }
   int   setLaw (Law* loi);

public:
    Propagation  ();
    void saveXml (XmlWriter& xml);
    void addEdge (Edge* arete);
    void majLaw  ();

private:
    Edges prop_edges;
    Law*  prop_law;
    bool  prop_way;
};
// =========================================================== Constructeur
inline Propagation::Propagation ()
{
    prop_law = NULL;
    prop_way = true;
}
// =========================================================== getLaw
inline Law* Propagation::getLaw ()
{
    return prop_law; 
/* **********************************  Modif Abu du 18/10/2010
    if (prop_law != NULL) 
       return prop_law; 

    if (prop_edges.size()==0) 
       return NULL;

    Document* root = prop_edges[0]->dad();
    return root->getLaw (0);
   ********************************** */
}
// =========================================================== setLaw
inline int Propagation::setLaw (Law* loi)
{
   prop_law = loi; 
   majLaw ();
   return HOK;
}
// =========================================================== majLaw
inline void Propagation::majLaw ()
{
   int nbre = prop_edges.size();
   for (int nro=0 ; nro < nbre ; nro++)
        prop_edges[0]->setLaw (prop_law);
}
// =========================================================== addEdge
inline void Propagation::addEdge (Edge* arete)
{
   prop_edges.push_back (arete);

   if (prop_law == NULL)
       prop_law =  arete->getLaw();
}
// =========================================================== saveXml
inline void Propagation::saveXml (XmlWriter& xml)
{
    char   buffer[12];
    cpchar booleen [] = { "true", "false" };
    cpchar law = prop_law == NULL ? "default" : prop_law->getName();

    xml.openMark     ("Propagation");
    xml.addAttribute ("edge", prop_edges[0]->getName (buffer));
    xml.addAttribute ("law",  law);
    xml.addAttribute ("way",  booleen [prop_way]);
    xml.closeMark ();
}
END_NAMESPACE_HEXA

#endif
