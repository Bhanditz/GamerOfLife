#include <GL/glew.h>

class Light {
 private:
    GLfloat posa, posb, posc, posd;// = { 4.0f, 4.0f, 4.0f, 1.0f };
    GLfloat amba, ambb, ambc, ambd;//  = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat difa, difb, difc, difd;//  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLenum id;
 public:

    Light(GLenum _id) {
//        position = { 0.0f, 0.0f, 0.0f, 1.0f };
//        ambient  = { 0.0f, 0.0f, 0.0f, 1.0f };
//        diffuse  = { 0.0f, 0.0f, 0.0f, 1.0f };
        this->id = _id; }

    void setPosition(GLfloat a, GLfloat b, GLfloat c, GLfloat d = 1.0f){
        this->posa = a;
        this->posb = b;
        this->posc = c;
        this->posd = d;
    }

    void setAmbient(GLfloat a, GLfloat b, GLfloat c, GLfloat d = 1.0f){
        this->amba = a;
        this->ambb = b;
        this->ambc = c;
        this->ambd = d;
    }

    void setDiffuse(GLfloat a, GLfloat b, GLfloat c, GLfloat d = 1.0f){
        this->difa = a;
        this->difb = b;
        this->difc = c;
        this->difd = d;
    }

    GLenum getId() { return this->id; }
//    void usePosition() { glLightfv(this->id, GL_POSITION, { posa, posb, posc, posd }); }
//    void useAmbient()  { glLightfv(this->id, GL_AMBIENT,  { amba, ambb, ambc, ambd }); }
//    void useDiffuse()  { glLightfv(this->id, GL_DIFFUSE,  { difa, difb, difc, difd }); }
};
