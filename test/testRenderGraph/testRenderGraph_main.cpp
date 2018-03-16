/*! *****************************************************************************
*   \file testRenderGraph_main.cpp
*   \author moennen
*   \brief 
*   \date 2018-03-02
*   *****************************************************************************/
#include "pbOptRenderPath.hpp"

#include "opencv2/highgui.hpp"  // command line parser

#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;
using namespace cv;

namespace
{
const string keys =
    "{help h usage ?   |         | print this message   }"
    "{@graphMLFilename |test.gml | graphML file         }";
};

int main( int argc, char* argv[] )
{
   CommandLineParser parser( argc, argv, keys );
   if ( parser.has( "help" ) )
   {
      parser.printMessage();
      return ( 0 );
   }

   // graph
   dynamic_properties graphProp;

   // file scope
   {
      string graphFilename = parser.get<string>( "@graphMLFilename" );
      ifstream graphFile( graphFilename );
      bool success = graphFile.good();
      if ( success )
      {
         success = read_graphviz( ifstream, graph, graphProp );
         if ( !success )
         {
            // reset the file
            ifstream.clear();
            ifstream.seekg( 0, ios::beg );
            success = read_graphml( ifstream, graph, graphProp );
         }
      }
      if ( !success )
      {
         cerr << "Cannot open : " << graphFilename << endl;
         return ( -1 );
      }
   }

   return ( 0 );
}
