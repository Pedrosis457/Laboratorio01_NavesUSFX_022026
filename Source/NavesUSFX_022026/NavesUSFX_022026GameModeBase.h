// ============================================================================
//  LABORATORIO 01 - Programacion Avanzada (SIS457)
//
//  GameMode del juego de Naves.
//  Aqui esta el requisito central del laboratorio: los objetos de la clase
//  Enemigo se colocan en la escena POR CODIGO C++ dentro del evento BeginPlay.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavesUSFX_022026GameModeBase.generated.h"

class AEnemigo;

UCLASS()
class NAVESUSFX_022026_API ANavesUSFX_022026GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANavesUSFX_022026GameModeBase();

	/** Evento begin del GameMode: desde aqui se colocan los enemigos. */
	virtual void BeginPlay() override;

protected:
	/** Crea por codigo los enemigos y los reparte por el escenario. */
	void ColocarEnemigosEnEscena();

	/** Crea un enemigo concreto en la posicion indicada. */
	AEnemigo* CrearEnemigo(const FVector& PosicionInicial, const FLinearColor& Color);

	/** Piso del escenario, generado tambien por codigo. */
	void PrepararEscenario();

	/** Camara fija que encuadra toda la zona de patrullaje. */
	void ColocarCamara();

	// ---------------------------------------------------------------- Parametros

	/** Cuantos enemigos se colocan al iniciar la partida. */
	UPROPERTY(EditAnywhere, Category = "Naves")
	int32 CantidadDeEnemigos;

	/** Medidas del escenario por el que patrullan los enemigos. */
	UPROPERTY(EditAnywhere, Category = "Naves")
	float AnchoDelEscenario;

	UPROPERTY(EditAnywhere, Category = "Naves")
	float LargoDelEscenario;

	UPROPERTY(EditAnywhere, Category = "Naves")
	float AlturaMinimaDeVuelo;

	UPROPERTY(EditAnywhere, Category = "Naves")
	float AlturaMaximaDeVuelo;

	/** Enemigos creados, guardados para poder consultarlos despues. */
	UPROPERTY()
	TArray<AEnemigo*> Enemigos;

	FTimerHandle TemporizadorCamara;
};
