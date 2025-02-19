#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>

namespace vdc {

	/**

	\class Grid
	Clase gen�rica que describe la interfaz com�n para trabajar con una malla. Las clases derivadas implementar�n
	los distintos tipos de malla (\sa UniformGrid, \sa RectilinearGrid, \sa StructuredGrid y \sa UnstructuredGrid)
	En todas las mallas, tanto las muestras como las celdas se identifican mediante un n�mero entero. Para las mallas
	uniformes, rectilineas y estructuradas, dicho identificador se corresponde con la ordenaci�n lexicogr�fica de
	sus coordenadas enteras (describe c�mo se han mapeado las distintas dimensiones en memoria).

	*/
	template <typename CoordType, typename SampleType>
	class Grid {
	public:
		virtual ~Grid() {};

		/**
		\return El n�mero de muestras (v�rtices) en la malla
		*/
		virtual size_t numSamples() const = 0;

		/**
		Devuelve la posici�n de la muestra indicada
		\param sampleIdx identificador de la muestra
		\return coordenadas de la muestra
		*/
		virtual CoordType getSamplePosition(size_t sampleIdx) const = 0;

		/**
		Devuelve el valor de la muestra indicada
		\param sampleIdx identificador de la muestra
		\return valor de la muestra
		*/
		SampleType getSampleValue(size_t sampleIdx) const { return samples[sampleIdx]; }

		/**
		Establece el valor de la muestra indicada
		\param sampleIdx identificador de la muestra
		\param value valor de la muestra
		*/
		void setSampleValue(size_t sampleIdx, const SampleType &value) {
			if (samples.size() < numSamples()) samples.resize(numSamples());
			samples[sampleIdx] = value;
		}

		/**
		\return El n�mero de celdas en la malla
		*/
		virtual size_t numCells() const = 0;
		/**
		Calcula los identificadores de los v�rtices correspondientes a la celda indicada.
		El orden de las muestras es: inferior izquierda, inferior derecha, superior derecha, 
		superior izquierda
		\param cellIdx �ndice de la celda
		\param vtsIdx variable donde guardar los �ndices de los v�rtices asociados a la celda
		  (debe tener suficiente espacio para almacenar el n�mero de v�rtices por celda)
		\return el n�mero de v�rtices escritos en vtsIdx (y n�mero de v�rtices por celda)
		*/
		virtual size_t getCellSamples(size_t cellIdx, size_t *vtsIdx) const = 0;
		/**
		Calcula el identificador de la celda correspondiente a la coordenada indicada
		\param p coordenadas del punto buscado
		\param cellIdx variable donde se escribir� el �ndice de la celda encontrada
		\return true si la celda se encuentra en el dataset, false en otro caso
		*/
		virtual bool findCell(const CoordType &p, size_t &cellIDx) const = 0;

		/**
		Implementa la transformaci�n que lleva de las coordenadas del mundo a las coordenadas
		de la celda referencia.
		\param c identificador de la celda con la que se est� trabajando
		\param world coordenadas en el sistema del mundo a transformar
		\return coordenadas en el sistema de referencia de la celda
		\warning Si el punto world no pertenece a la celda, alguna de las dimensiones de cell estar�
		fuera del rango [0..1]
		*/
		virtual CoordType world2cell(size_t c, const CoordType & world) const {
			return CoordType(-1.f);
		};

		/**
		Devuelve el valor en el punto indicado con una interpolaci�n lineal.
		Asume celdas con cuatro v�rtices
		\param cell identificador de la celda
		\param refcoords coordenadas en la celda de referencia
		*/
		SampleType interpolateC1Square(size_t cell, const CoordType &refcoords) const {
			SampleType result;
			size_t vtcs[4];
			auto nv = getCellSamples(cell, vtcs);
			assert(nv == 4);
			result =
				(1 - refcoords.x)*(1 - refcoords.y)*getSampleValue(vtcs[0]) +
				refcoords.x * (1 - refcoords.y)*getSampleValue(vtcs[1]) +
				refcoords.x * refcoords.y * getSampleValue(vtcs[2]) +
				(1 - refcoords.x)*refcoords.y * getSampleValue(vtcs[3]);
			return result;
		}
	protected:
		std::vector<SampleType> samples;
	};
};
