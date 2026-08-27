// ============================================================================
//  LABORATORIO 01 - Programacion Avanzada (SIS457)
//  Universidad San Francisco Xavier de Chuquisaca
//
//  Clase Enemigo: nave enemiga que se desplaza por diferentes partes
//  del escenario eligiendo destinos al azar dentro de su zona de patrullaje.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemigo.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class NAVESUSFX_022026_API AEnemigo : public AActor
{
	GENERATED_BODY()

public:
	AEnemigo();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Define el volumen del escenario por el que este enemigo puede moverse.
	 * Lo llama el GameMode justo despues de crear al enemigo.
	 */
	void DefinirZonaDePatrullaje(const FVector& NuevoCentro, float NuevoAncho, float NuevoLargo,
		float NuevaAlturaMinima, float NuevaAlturaMaxima);

	/** Color de la nave, para poder distinguir unas de otras en el escenario. */
	void EstablecerColor(const FLinearColor& NuevoColor);

	FVector ObtenerDestino() const { return Destino; }

protected:
	virtual void BeginPlay() override;

	/** Sortea un nuevo punto del escenario al cual dirigirse. */
	void ElegirNuevoDestino();

	/** Avanza hacia el destino actual y gira suavemente hacia el. */
	void MoverHaciaDestino(float DeltaTime);

	// ---------------------------------------------------------------- Componentes

	/**
	 * Raiz del actor. Es importante que la raiz NO sea la malla: el cabeceo se
	 * aplica moviendo la malla en local, y si la malla fuera la raiz ese
	 * movimiento sobrescribiria la posicion del actor en el mundo.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Enemigo|Componentes")
	USceneComponent* Raiz;

	UPROPERTY(VisibleAnywhere, Category = "Enemigo|Componentes")
	UStaticMeshComponent* Malla;

	// ---------------------------------------------------------------- Movimiento

	/** Velocidad de avance en cm/s. */
	UPROPERTY(EditAnywhere, Category = "Enemigo|Movimiento")
	float Velocidad;

	/** Que tan rapido endereza el morro hacia el destino. */
	UPROPERTY(EditAnywhere, Category = "Enemigo|Movimiento")
	float VelocidadDeGiro;

	/** A que distancia se considera que ya llego y toca elegir otro destino. */
	UPROPERTY(EditAnywhere, Category = "Enemigo|Movimiento")
	float DistanciaDeLlegada;

	/** Cabeceo vertical, para que el vuelo no se vea rigido. */
	UPROPERTY(EditAnywhere, Category = "Enemigo|Movimiento")
	float AmplitudDeCabeceo;

	UPROPERTY(VisibleAnywhere, Category = "Enemigo|Movimiento")
	FVector Destino;

	// ---------------------------------------------------------------- Zona de patrullaje

	UPROPERTY(EditAnywhere, Category = "Enemigo|Zona")
	FVector CentroDeZona;

	UPROPERTY(EditAnywhere, Category = "Enemigo|Zona")
	float AnchoDeZona;

	UPROPERTY(EditAnywhere, Category = "Enemigo|Zona")
	float LargoDeZona;

	UPROPERTY(EditAnywhere, Category = "Enemigo|Zona")
	float AlturaMinima;

	UPROPERTY(EditAnywhere, Category = "Enemigo|Zona")
	float AlturaMaxima;

private:
	float TiempoVivo;

	FLinearColor Color;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDinamico;
};
