#include "Renderer.h"
#include "GraphicsAPI.h"
#include "VertexArray.h"
#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

#include "opengl/OpenGLShader.h"

namespace engine {
    void Renderer::BeginScene(OrthographicCamera& camera) {
        // TODO: set camera, light, shader setting etc.
        _viewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {
        // TODO: batch rendering, etc.
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
        // this is a temporary solution
        auto glShader = std::static_pointer_cast<OpenGLShader>(shader);

        glShader->Bind();
        glShader->SetUniformMat4("u_viewProjection", _viewProjectionMatrix);
        glShader->SetUniformMat4("u_transform", transform);

        _renderCommand->DrawIndexed(vertexArray);
    }
}