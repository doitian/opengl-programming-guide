#version 450 core

layout(binding = 0) uniform Uniforms {
  vec4 color;
} ubo;

layout(location = 0) out vec4 fColor;

void main() {
  fColor = ubo.color;
}
