#ifndef __ModelTest__
#define __ModelTest__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "hexa_base.hxx"
// #include "Hex.hxx"
#include "HexDocument.hxx"
#include "HexEltBase.hxx"

class ModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( ModelTest );
    CPPUNIT_TEST( myTest_croix);
    CPPUNIT_TEST( Test_croix);
    CPPUNIT_TEST( Test_disconnect);
    CPPUNIT_TEST( Test_joint);
    CPPUNIT_TEST( Test_prism);
    CPPUNIT_TEST( Test_cylinder);
    CPPUNIT_TEST( Test_find);
    CPPUNIT_TEST( Test_lorraine);
    CPPUNIT_TEST( Test_propagation);
    CPPUNIT_TEST( Test_cartesi1);
    CPPUNIT_TEST( Test_decoupage);
    CPPUNIT_TEST( Test_grille_cyl);
    CPPUNIT_TEST( Test_pipes);
    CPPUNIT_TEST( Test_separ);
    CPPUNIT_TEST( Test_clone);
    CPPUNIT_TEST( Test_deux_cyl);
    CPPUNIT_TEST( Test_hexa1);
    CPPUNIT_TEST( Test_prism2);
    CPPUNIT_TEST( Test_transfo);
    CPPUNIT_TEST( Test_hexa1);
    CPPUNIT_TEST( Test_move);
    CPPUNIT_TEST( Test_gen_xml);
    CPPUNIT_TEST( Test_relecture);
    CPPUNIT_TEST_SUITE_END();
  
  public:
    ModelTest(void);
    ~ModelTest(void);
    //~ Call before tests
    void setUp(void) {}
    //~ Call after tests
    void tearDown(void) {}

//     void Test_bielle();
    void myTest_croix();
    void Test_croix();
    void Test_disconnect();
    void Test_joint();
    void Test_prism();
//     void Test_bride();
    void Test_cylinder();
    void Test_find();
    void Test_lorraine();
    void Test_propagation();
//     void Test_bug1();
//     void Test_debride();
    void Test_gen_xml();
    void Test_move();
    void Test_relecture();
    void Test_cartesi1();
    void Test_decoupage();
    void Test_grille_cyl();
    void Test_pipes();
    void Test_separ();
    void Test_clone();
    void Test_deux_cyl();
    void Test_hexa1();
    void Test_prism2();
    void Test_transfo();

  private:
    int _afficher (cpchar nom, Hex::EltBase* elt);
    void _print_propagations (Hex::Document* doc);
//     HEXA_NS::Hex *_myHexEngine;
//     HEXA_NS::Document *_myDoc;

};

#endif
