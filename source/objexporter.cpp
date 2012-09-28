#include "objexporter.h"
#include "fstream"
#include "iostream"

using namespace std;

void ExportModel( string filepath, Mesh* model )
{

    // --- update vertex indices ---
    for( int i = 0; i < model->vertices.size(); i++)
        model->vertices[i]->index = i;

    ofstream myfile;
    myfile.open(filepath.c_str());
    cout << filepath << endl;
    if(model->name == "" )
        model->name = "Tree Mesh";
    myfile << "g " << model->name << endl;

    myfile << endl;

    vector < Vertex* >& vertices = model->vertices;
    for( unsigned int i = 0; i < vertices.size(); i++)
        myfile << "v " << vertices[i]->position.x <<  " " << vertices[i]->position.y << " " <<  vertices[i]->position.z << endl;

     myfile << endl;

      vector < Face* >& quads = model->quads;

      for( unsigned int i = 0; i < quads.size(); i++)
      {
          myfile << "f ";
          Face* face =quads[i];
          for( unsigned int j = 0; j < 4; j++)
              myfile << (face->vertices[j]->index + 1) << " ";

          myfile << endl;
      }

      vector < Face* >& tris = model->triangles;

      for( unsigned int i = 0; i < tris.size(); i++)
      {
          myfile << "f ";
          Face* face =tris[i];
          for( unsigned int j = 0; j < 3; j++)
              myfile << (face->vertices[j]->index +1)<< " ";

          myfile << endl;
      }


      myfile.close();
}
