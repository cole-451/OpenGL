#pragma once
#include "Resources/Resource.h"
#include <glm/glm.hpp>
#include "GUI.h"
namespace neu {
	class Program;
	class Texture;

	class Material : public Resource, GUI {
	public:

		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();
		// Inherited via GUI
		void UpdateGui() override;

	public:
		float shininess = 2;
		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1,1,1 };
		res_t<Texture> specularMap;
		res_t<Program> program;
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };

	};


}