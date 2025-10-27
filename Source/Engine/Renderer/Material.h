#pragma once
#include "Resources/Resource.h"
#include <glm/glm.hpp>
namespace neu {
	class Program;
	class Texture;

	class Material : public Resource {
	public:

		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

	public:
		float shininess = 2;
		res_t<Texture> texture;
		res_t<Program> program;
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };
	};


}