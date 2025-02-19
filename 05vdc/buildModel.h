#include <glm/gtc/matrix_transform.hpp>

#include <PGUPV.h>

#include "grid.h"
#include "uniformGrid.h"
#include "rectilinearGrid.h"
#include "structuredGrid.h"
#include "unstructuredTriangleGrid.h"

using namespace PGUPV;

namespace vdc {

	/**
	Dada una malla de dimensi�n 2 y atributos de tipo float, devuelve un modelo que muestra dicha malla
	como un mapa de alturas, donde la altura es el valor del atributo en cada muestra.
	\param g la malla
	\return el modelo PGUPV que representa la malla
	*/
	std::shared_ptr<PGUPV::Model> build2DHeightMapFromGrid(const Grid<glm::vec2, float> &g);

	/**
	Construye una malla uniforme del tama�o indicado.
	*/
	std::unique_ptr<vdc::UniformGrid<glm::vec2, float>> buildUniformGrid(const int horizontalVtcs, const int verticalVtcs, const glm::vec2& min, const glm::vec2& max);

	/**
	Construye una malla rectil�nea del tama�o indicado.
	*/
	std::unique_ptr<vdc::RectilinearGrid<glm::vec2, float>>  buildRectilinearGrid(const int horizontalVtcs, const int verticalVtcs, const glm::vec2& min, const glm::vec2& max);

	/**
	Construye una malla estructurada del tama�o indicado.
	*/
	std::unique_ptr<vdc::StructuredGrid<glm::vec2, float>> buildStructuredGrid(const int horizontalVtcs, const int verticalVtcs, const glm::vec2& min, const glm::vec2& max);

	/**
	Construye una malla no estructurada del tama�o indicado.
	*/
	std::unique_ptr<vdc::UnstructuredTriangleGrid<glm::vec2, float>> buildUnstructuredGrid(const int horizontalVtcs, const int verticalVtcs, const glm::vec2& min, const glm::vec2& max);


	/**
	Dada una malla uniforme (o rectil�nea o estructurada, que derivan de la primera) de dimensi�n 2, con un atributo de 
	tipo float (que puede representar, por ejemplo, la altura), devuelve una malla estructurada de dimensi�n 2, con un 
	atributo de tipo vec2, que indica para muestra la derivada de la funci�n en dicho punto.

	Tendr�s que implementar el algoritmo de derivaci�n visto en clase.

	\param g malla uniforme (o rectil�nea) de dimensi�n 2 y muestras escalares
	\return una malla estructurada con las muestras en las mismas posiciones que la malla de entrada, y con el valor
	de la muestra conteniendo el valor de las derivadas parciales con respecto a x e y
	*/
	std::shared_ptr<StructuredGrid<glm::vec2, glm::vec2>> derivative(const UniformGrid<glm::vec2, float> &g);

	/**
	Dada una malla con una dimensi�n n, y con un atributo de la misma dimensi�n, devuelve un modelo OpenGL que la
	representa mediante l�neas. Cada l�nea empieza en las coordenadas de la muestra, y va en la direcci�n del atributo.
	\param g la malla a dibujar (de cualquier tipo, aunque la dimensi�n de la malla y su atributo deben coincidir)
	\param mult el tama�o de la l�nea (la longitud del vector atributo) se multiplicar� por este valor
	\param max longitud m�xima de las l�neas
	
	*/
	template<typename CoordType>
	std::shared_ptr<Model> buildVectorModelFromGrid(const Grid<CoordType, CoordType> &g, float mult, float max) {
		auto model = std::make_shared<Model>();

		std::vector<CoordType> coordinates;
		for (size_t i = 0; i < g.numSamples(); i++) {
			auto p = g.getSamplePosition(i);
			coordinates.push_back(p);
			coordinates.push_back(p + g.getSampleValue(i) * std::min(max, mult * glm::length(g.getSampleValue(i))));
		}

		// Malla
		auto base = std::make_shared<Mesh>();
		base->addVertices(coordinates);

		base->addDrawCommand(new DrawArrays(GL_LINES, 0, static_cast<GLsizei>(coordinates.size())));
		model->addMesh(base);
		return model;
	}

};