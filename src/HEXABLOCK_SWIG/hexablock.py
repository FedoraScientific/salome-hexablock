# -*- coding: latin-1 -*-
# HexaBlock : Module principal

import hexablock_swig
import salome
import smesh

import HEXABLOCKPlugin

geompy    = smesh.geompy
component = hexablock_swig.hex_instance ()

# ======================================================== moduleName
# ### Gives the component name
def moduleName () :
    return "HEXABLOCK"

# ======================================================== getEngine
# ### Gives the component 
def getEngine () :
    return component

# ======================================================== what
def what () :
   component.what ()

# ======================================================== countDocument
def countDocument () :
   return component.countDocument ()

# ======================================================== getDocument
def getDocument (nro) :
   return  component.getDocument (nro)

# ======================================================== removeDocument
def removeDocument (doc) :
   return  component.removeDocument (doc)

# ======================================================== addDocument
def addDocument (nomdoc) :
   return  component.addDocument (nomdoc)

# ======================================================== loadDocument
def loadDocument (filename) :
   return  component.loadDocument (filename)

# ======================================================== findDocument
def findDocument (name) :
   return  component.findDocument (name)

# ======================================================== dump
# ### Display informations about a document
def dump(doc, mesh=None, full=False):
    if full:
        hn = doc.countUsedHexa()
        print "Model dump: number of hexas: ", hn
        for hi in xrange(hn):
            hh = doc.getUsedHexa(hi)
            print "  hexa: ", hi, "name: ", hh.getName()
            for fi in xrange(6):
                ff = hh.getQuad(fi)
                fa = ff.getAssociations()
                print "    quadrangle: ", fi, "name: ", ff.getName(), " associated: ", fa!=[]
                for ei in xrange(4):
                    ee = ff.getEdge(ei)
                    ea = ee.getAssociations()
                    print "      edge: ", ei, "name: ", ee.getName(), " associated: ", ea!=[]
                    for vi in xrange(2):
                        vv = ee.getVertex(vi)
                        va = vv.getAssociation()
                        print "        vertex: ", vi, "name: ", vv.getName(), " associated: ", va!=None
                        print "          model: x= ", vv.getX(), " y= ", vv.getY(), " z= ", vv.getZ()
                        if va!=None:
                            x, y, z = geompy.PointCoordinates(va)
                            print "          assoc: x= ", x, " y= ", y, " z= ", z

    uv = doc.countUsedVertex()
    ue = doc.countUsedEdge()
    uq = doc.countUsedQuad()
    uh = doc.countUsedHexa()

    print "Model vertices    number: ", uv
    print "Model edges       number: ", ue
    print "Model quadrangles number: ", uq
    print "Model blocks      number: ", uh

    if mesh != None:
        print 
        print "Mesh nodes       number: ", mesh.NbNodes()
        print "Mesh segments    number: ", mesh.NbEdges()
        print "Mesh quadrangles number: ", mesh.NbQuadrangles()
        print "Mesh hexas       number: ", mesh.NbHexas()

    return uv, ue, uq, uh

# ======================================================== mesh
# ### Mesh a document
def mesh (doc, name=None, dim=3, container="FactoryServer"):
    study = salome.myStudy

    if type(doc) == type(""):
        doc = component.findDocument (doc)
    docname = doc.getName()

    ####  if doc.countShape() == 0 :
       ####  shape = geompy.MakeBox(0, 0, 0,  1, 1, 1)
    ####  else :
       ####  shape = doc.getShape (0)

    shape = geompy.MakeBox(0, 0, 0,  1, 1, 1)
    if (name == None) or (name == ""):
        name = docname

    geompy.addToStudy(shape, name)
    comp_smesh = salome.lcc.FindOrLoadComponent(container, "SMESH")
    comp_smesh.init_smesh(study, geompy.geom)
    meshexa = comp_smesh.Mesh(shape)

    so = "libHexaBlockEngine.so"

    algo = smesh.SMESH._objref_SMESH_Gen.CreateHypothesis(comp_smesh, "HEXABLOCK_3D", so)
    meshexa.mesh.AddHypothesis(shape, algo)

    hypo = smesh.SMESH._objref_SMESH_Gen.CreateHypothesis(comp_smesh, "HEXABLOCK_Parameters", so)
    meshexa.mesh.AddHypothesis(shape, hypo)

    ### hypo.SetDocument(doc.getXml())   ## Hexa6 TODO et a verifier
    hypo.SetDocument (docname);
    hypo.SetDimension(dim)

    meshexa.Compute()

    return meshexa

# ======================================================== getFromStudy
def getFromStudy(entry):
    study    = salome.myStudy
    sobject  = study.FindObjectID(entry)
    if sobject == None :
       print " **** Entry ", entry, " is undefined"
       return None

    builder  = study.NewBuilder()
    ok, attname = builder.FindAttribute(sobject, "AttributeName")
    docname  = attname.Value()
    doc = component.findDocument(docname)
    if doc == None :
       print " **** Entry ", entry, " doesn't correspond to an HexaBlock Document"

    return doc

# ==================================================== findOrCreateComponent
# Find or create HexaBlock Study Component
def findOrCreateComponent( study, builder ):
    father = study.FindComponent( moduleName() )
    if father is None:
       father = builder.NewComponent( moduleName() )
       attr = builder.FindOrCreateAttribute( father, "AttributeName" )
       attr.SetValue( "HexaBlock" )
       attr = builder.FindOrCreateAttribute( father, "AttributePixMap" )
       attr.SetPixMap( "ICO_MODULE_HEXABLOCK_SMALL" )

    return father

# ==================================================== addToStudy
# Add a document in the current study
def addToStudy(doc):
    if doc == None :
       print " *** addToStudy : Bad Document Pointer"
       return

    study   = salome.myStudy
    builder = study.NewBuilder()
    father  = findOrCreateComponent( study, builder )
    name    = doc.getName ()

    present = study.FindObjectByName(name, moduleName())
    if present != [] :
       print " *** addToStudy : Document ", name, "is already in the study"
       return

    object  = builder.NewObject( father )
    attr    = builder.FindOrCreateAttribute( object, "AttributeName" )
    attr.SetValue( name )
    return object.GetID ()

# ==================================================== addShape
# Add a document in the current study
def addShape (doc, shape, name):
    doc.addShape (shape.getShape(), name)