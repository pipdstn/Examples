#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <iostream>
#include <stdio.h>
#include <ctime>

NGLScene::NGLScene() {
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Snowflaker");
}

NGLScene::~NGLScene() {
  std::cout<<"Shutting down NGL, removing VAOs and Shaders\n";
}

void NGLScene::resizeGL(int _w , int _h) {
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_text->setScreenSize(_w,_h);
}

void NGLScene::initializeGL() {
  ngl::NGLInit::initialize();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_PROGRAM_POINT_SIZE);

  ngl::ShaderLib::use(ngl::nglColourShader);

  m_view = ngl::lookAt({0, 0, 12}, {0, 0, 0}, {0, 1, 0});
  m_project = ngl::perspective(45.0f, 1.0f, 0.01f, 50.0f);

  ngl::ShaderLib::loadShader("SnowflakeShader", "shaders/SnowflakeVertex.glsl", "shaders/SnowflakeFragment.glsl");
  ngl::ShaderLib::use("SnowflakeShader");

  m_snowflake = std::make_unique<Snowflake>();
  m_text = std::make_unique<ngl::Text>("fonts/Monaco.ttf", 16);
}

void NGLScene::renderControlsText(int x, int y) {
  auto controlsText = "1 = Generate new snowflake; SPACEBAR = export current snowflake";
  m_text->setColour(1.0f, 1.0f, 1.0f);
  m_text->renderText(x, y, controlsText);
}

void NGLScene::renderInfoText(int x, int y) {
  auto infoText = "# of Particles = " + std::to_string(m_snowflake->m_particles.size()) + "; ";
  infoText += "Y-bias = ";
  infoText += fmt::format("{:0.5f}", m_snowflake->bias / m_snowflake->particle_radius);
  m_text->setColour(1.0f, 1.0f, 1.0f);
  m_text->renderText(x, y, infoText);
}

void NGLScene::renderExportText(int x, int y, std::string filename) {
  auto exportText = "Exported to file " + filename;
  m_text->setColour(0.1f, 1.0f, 0.1f);
  m_text->renderText(x, y, exportText);
}

void NGLScene::paintGL() {
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);

  ngl::ShaderLib::use("SnowflakeShader");  
  ngl::ShaderLib::setUniform("MVP", m_project * m_view);
  
  if (m_snowflake->num_particles < m_snowflake->max_particles) {
    m_snowflake->update();
  }
  m_snowflake->render();
  renderControlsText(10, 700);
  renderInfoText(10, 680);
  
  if (m_snowflake->exported) {
    renderExportText(10, 20, m_snowflake->exportedName);
  }
  
  update();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event) {
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_Escape : 
    QGuiApplication::exit(EXIT_SUCCESS); 
    break;
  case Qt::Key_1 :
    m_snowflake->reset();
    break;
  case Qt::Key_Space :
    if (m_snowflake->num_particles == m_snowflake->max_particles) {
      m_snowflake->exportFlake();
    }
  default : break;
  }
  // finally update the GLWindow and re-draw
    update();
}
