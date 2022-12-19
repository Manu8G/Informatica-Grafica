class luz{    
  public:

    float x, y, z, grax, gray, graz;
    int a, b, c;
    GLenum indice_luz;
    _vertex4f punto_luz;
    _vertex4f luz_ambiente;
    _vertex4f luz_difusa;
    _vertex4f luz_especular;


  luz(GLenum indice_luz, _vertex4f punto_luz, _vertex4f luz_ambiente, _vertex4f luz_difusa, _vertex4f luz_especular){
    this->indice_luz = indice_luz;
    this->punto_luz = punto_luz;
    this->luz_ambiente = luz_ambiente;
    this->luz_difusa = luz_difusa;
    this->luz_especular = luz_especular;

    x = punto_luz[0];
    y = punto_luz[1];
    z = punto_luz[2];
    grax = 0;
    gray = 0;
    graz = 0;
    a = 0;
    b = 0;
    c = 0;
  }

  void activar(){
    glEnable(GL_LIGHTING);
    glEnable(indice_luz);
    glLightfv(indice_luz, GL_AMBIENT, (GLfloat*) &luz_ambiente);
    glLightfv(indice_luz, GL_DIFFUSE, (GLfloat*) &luz_difusa);
    glLightfv(indice_luz, GL_SPECULAR, (GLfloat*) &luz_especular);
    glLightfv(indice_luz, GL_POSITION, (GLfloat*) &punto_luz);
  }

  void desactivar(){
    glDisable(indice_luz);
    glDisable(GL_LIGHTING);	
  }
};

