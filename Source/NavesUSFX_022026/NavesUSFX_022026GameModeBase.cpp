// ============================================================================
//  LABORATORIO 01 - Programacion Avanzada (SIS457)
//  Implementacion del GameMode.
// ============================================================================

#include "NavesUSFX_022026GameModeBase.h"

#include "Enemigo.h"

#include "Camera/CameraActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

ANavesUSFX_022026GameModeBase::ANavesUSFX_022026GameModeBase()
{
	CantidadDeEnemigos = 8;

	AnchoDelEscenario = 6000.0f;
	LargoDelEscenario = 6000.0f;
	AlturaMinimaDeVuelo = 200.0f;
	AlturaMaximaDeVuelo = 1100.0f;
}

void ANavesUSFX_022026GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PrepararEscenario();

	// ==========================================================
	//  REQUISITO DEL LABORATORIO 01
	//  Los objetos de la clase Enemigo se colocan en la escena
	//  por codigo C++, aqui en el evento begin del GameMode.
	// ==========================================================
	ColocarEnemigosEnEscena();

	// La camara se ubica con un retardo minimo, cuando el PlayerController ya existe.
	GetWorldTimerManager().SetTimer(TemporizadorCamara, this,
		&ANavesUSFX_022026GameModeBase::ColocarCamara, 0.2f, false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan,
			FString::Printf(TEXT("%d enemigos colocados por codigo C++ desde el BeginPlay del GameMode"),
				Enemigos.Num()));
	}
}

void ANavesUSFX_022026GameModeBase::ColocarEnemigosEnEscena()
{
	Enemigos.Empty();

	// Una paleta para que cada nave se distinga de las demas.
	const TArray<FLinearColor> Paleta = {
		FLinearColor(0.95f, 0.20f, 0.20f),   // rojo
		FLinearColor(0.20f, 0.55f, 0.95f),   // azul
		FLinearColor(0.20f, 0.85f, 0.35f),   // verde
		FLinearColor(0.95f, 0.75f, 0.10f),   // amarillo
		FLinearColor(0.80f, 0.30f, 0.90f),   // violeta
		FLinearColor(0.10f, 0.85f, 0.85f),   // turquesa
		FLinearColor(0.95f, 0.50f, 0.10f),   // naranja
		FLinearColor(0.90f, 0.90f, 0.90f)    // blanco
	};

	for (int32 i = 0; i < CantidadDeEnemigos; ++i)
	{
		// Posicion inicial: repartidos en circulo para que no nazcan encimados.
		const float Angulo = (2.0f * PI * i) / FMath::Max(1, CantidadDeEnemigos);
		const float Radio = 1800.0f;

		const FVector PosicionInicial(
			FMath::Cos(Angulo) * Radio,
			FMath::Sin(Angulo) * Radio,
			FMath::FRandRange(AlturaMinimaDeVuelo, AlturaMaximaDeVuelo));

		const FLinearColor Color = Paleta[i % Paleta.Num()];

		if (AEnemigo* Nuevo = CrearEnemigo(PosicionInicial, Color))
		{
			Enemigos.Add(Nuevo);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] %d enemigos colocados por codigo en el BeginPlay"), Enemigos.Num());
}

AEnemigo* ANavesUSFX_022026GameModeBase::CrearEnemigo(const FVector& PosicionInicial, const FLinearColor& Color)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return nullptr;
	}

	// Spawn diferido: permite configurar la zona de patrullaje y el color
	// ANTES de que corra el BeginPlay del enemigo, que es donde elige
	// su primer destino y aplica su material.
	const FTransform Transformacion(FRotator::ZeroRotator, PosicionInicial);

	AEnemigo* Nuevo = Mundo->SpawnActorDeferred<AEnemigo>(AEnemigo::StaticClass(), Transformacion);
	if (!Nuevo)
	{
		return nullptr;
	}

	Nuevo->DefinirZonaDePatrullaje(FVector::ZeroVector, AnchoDelEscenario, LargoDelEscenario,
		AlturaMinimaDeVuelo, AlturaMaximaDeVuelo);
	Nuevo->EstablecerColor(Color);

	UGameplayStatics::FinishSpawningActor(Nuevo, Transformacion);

	return Nuevo;
}

void ANavesUSFX_022026GameModeBase::PrepararEscenario()
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return;
	}

	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Piso = Mundo->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Parametros);
	if (!Piso)
	{
		return;
	}

	UStaticMeshComponent* Componente = NewObject<UStaticMeshComponent>(Piso);
	Componente->SetMobility(EComponentMobility::Movable);
	Componente->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UStaticMesh* MallaPlano = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane")))
	{
		Componente->SetStaticMesh(MallaPlano);
	}

	if (UMaterial* MaterialBasico = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
	{
		Componente->SetMaterial(0, MaterialBasico);
	}

	// El plano basico mide 100 uu, se escala para cubrir todo el escenario.
	Componente->SetRelativeScale3D(FVector(AnchoDelEscenario / 100.0f, LargoDelEscenario / 100.0f, 1.0f));

	Piso->SetRootComponent(Componente);
	Componente->RegisterComponent();

	if (UMaterialInstanceDynamic* Dinamico = Componente->CreateAndSetMaterialInstanceDynamic(0))
	{
		Dinamico->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.14f, 0.17f, 0.22f));
	}
}

void ANavesUSFX_022026GameModeBase::ColocarCamara()
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return;
	}

	const FVector PosicionCamara(0.0f, -6200.0f, 3000.0f);
	const FRotator RotacionCamara(-22.0f, 90.0f, 0.0f);

	ACameraActor* Camara = Mundo->SpawnActor<ACameraActor>(PosicionCamara, RotacionCamara);
	if (!Camara)
	{
		return;
	}

	if (APlayerController* Controlador = UGameplayStatics::GetPlayerController(this, 0))
	{
		Controlador->SetViewTargetWithBlend(Camara, 0.0f);
	}
}
