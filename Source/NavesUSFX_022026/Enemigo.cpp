// ============================================================================
//  LABORATORIO 01 - Programacion Avanzada (SIS457)
//  Implementacion de la clase Enemigo.
// ============================================================================

#include "Enemigo.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AEnemigo::AEnemigo()
{
	PrimaryActorTick.bCanEverTick = true;

	// --- Raiz del actor ------------------------------------------------------
	// La raiz es un componente vacio, no la malla. Asi el cabeceo puede mover
	// la malla en local sin alterar la posicion del actor en el mundo.
	Raiz = CreateDefaultSubobject<USceneComponent>(TEXT("Raiz"));
	RootComponent = Raiz;

	// --- Cuerpo visible de la nave -------------------------------------------
	Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
	Malla->SetupAttachment(Raiz);
	Malla->SetMobility(EComponentMobility::Movable);
	Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Se usa un cono como fuselaje: apunta hacia +Z, asi que se lo inclina
	// -90 grados para que el morro quede mirando hacia adelante (+X).
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCono(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (MallaCono.Succeeded())
	{
		Malla->SetStaticMesh(MallaCono.Object);
	}
	Malla->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Malla->SetRelativeScale3D(FVector(0.45f, 0.45f, 1.10f));

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBasico(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MaterialBasico.Succeeded())
	{
		Malla->SetMaterial(0, MaterialBasico.Object);
	}

	// --- Valores por defecto del movimiento ----------------------------------
	Velocidad = 550.0f;
	VelocidadDeGiro = 3.0f;
	DistanciaDeLlegada = 150.0f;
	AmplitudDeCabeceo = 25.0f;
	Destino = FVector::ZeroVector;

	// --- Zona de patrullaje por defecto --------------------------------------
	CentroDeZona = FVector::ZeroVector;
	AnchoDeZona = 6000.0f;
	LargoDeZona = 6000.0f;
	AlturaMinima = 200.0f;
	AlturaMaxima = 1100.0f;

	TiempoVivo = 0.0f;
	Color = FLinearColor(0.9f, 0.2f, 0.2f);
	MaterialDinamico = nullptr;
}

void AEnemigo::DefinirZonaDePatrullaje(const FVector& NuevoCentro, float NuevoAncho, float NuevoLargo,
	float NuevaAlturaMinima, float NuevaAlturaMaxima)
{
	CentroDeZona = NuevoCentro;
	AnchoDeZona = NuevoAncho;
	LargoDeZona = NuevoLargo;
	AlturaMinima = NuevaAlturaMinima;
	AlturaMaxima = NuevaAlturaMaxima;
}

void AEnemigo::EstablecerColor(const FLinearColor& NuevoColor)
{
	Color = NuevoColor;

	// Si ya esta en juego, se aplica de inmediato.
	if (MaterialDinamico)
	{
		MaterialDinamico->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void AEnemigo::BeginPlay()
{
	Super::BeginPlay();

	if (Malla)
	{
		MaterialDinamico = Malla->CreateAndSetMaterialInstanceDynamic(0);
		if (MaterialDinamico)
		{
			MaterialDinamico->SetVectorParameterValue(TEXT("Color"), Color);
		}
	}

	// Primer destino: arranca a recorrer el escenario apenas empieza el juego.
	ElegirNuevoDestino();

	UE_LOG(LogTemp, Warning, TEXT("[Enemigo] %s en juego. Primer destino: %s"),
		*GetName(), *Destino.ToCompactString());
}

void AEnemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TiempoVivo += DeltaTime;

	MoverHaciaDestino(DeltaTime);

	// Cabeceo suave del fuselaje, para que el vuelo no se vea rigido.
	if (Malla)
	{
		const float Cabeceo = FMath::Sin(TiempoVivo * 2.2f) * AmplitudDeCabeceo;
		Malla->SetRelativeLocation(FVector(0.0f, 0.0f, Cabeceo));
	}
}

void AEnemigo::ElegirNuevoDestino()
{
	// Un punto cualquiera dentro del volumen asignado: de ahi que el enemigo
	// termine recorriendo diferentes partes del escenario y no una ruta fija.
	Destino = CentroDeZona + FVector(
		FMath::FRandRange(-AnchoDeZona * 0.5f, AnchoDeZona * 0.5f),
		FMath::FRandRange(-LargoDeZona * 0.5f, LargoDeZona * 0.5f),
		0.0f);

	Destino.Z = FMath::FRandRange(AlturaMinima, AlturaMaxima);
}

void AEnemigo::MoverHaciaDestino(float DeltaTime)
{
	const FVector Posicion = GetActorLocation();
	FVector Direccion = Destino - Posicion;
	const float Distancia = Direccion.Size();

	// Llego: toca elegir otra parte del escenario.
	if (Distancia <= DistanciaDeLlegada)
	{
		const FVector Alcanzado = Destino;
		ElegirNuevoDestino();

		UE_LOG(LogTemp, Warning, TEXT("[Enemigo] %s llego a %s, nuevo destino %s"),
			*GetName(), *Alcanzado.ToCompactString(), *Destino.ToCompactString());

		return;
	}

	Direccion /= Distancia;

	// Avance.
	SetActorLocation(Posicion + Direccion * Velocidad * DeltaTime);

	// Giro progresivo hacia el rumbo, en lugar de un cambio brusco.
	const FRotator RotacionObjetivo = Direccion.Rotation();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, VelocidadDeGiro));
}
