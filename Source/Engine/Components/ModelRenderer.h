#pragma once
#include "RendererComponent.h"
namespace neu {
	class ModelRenderer : public RendererComponent {
	public:
		CLASS_PROTOTYPE(ModelRenderer)

	public:
		res_t<Model> model;
		res_t<Material> material;

		// Inherited via RendererComponent
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;
		void Read(const serial_data_t& value);
		void UpdateGui() override;

		// Inherited via RendererComponent
		const char* GetClassName() override;
	};
}