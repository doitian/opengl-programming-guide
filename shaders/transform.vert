#version 450 core

layout(binding = 0) uniform Transform {
  mat4 modelMatrix;
  mat4 projectionMatrix;
};

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

out vec4 color;

void main(void) {
  color = vColor;
  gl_Position = projectionMatrix * (modelMatrix * vPosition);
}
