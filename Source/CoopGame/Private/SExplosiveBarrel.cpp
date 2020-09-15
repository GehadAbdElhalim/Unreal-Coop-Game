// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->bApplyImpulseOnDamage = false;
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	SetRootComponent(MeshComp);

	RadialComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialComp"));
	RadialComp->bIgnoreOwningActor = true;
	RadialComp->bAutoActivate = false;
	RadialComp->SetupAttachment(MeshComp);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	bDidExplode = false;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHealthChanged);
}

void ASExplosiveBarrel::OnRep_Exploded()
{
	//play particles
	UGameplayStatics::SpawnEmitterAttached(effect, RootComponent);

	//Change Material
	MeshComp->SetMaterial(0, ExplodedMaterial);
}

void ASExplosiveBarrel::OnBarrelHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bDidExplode)
	{
		return;
	}

	if (Health <= 0)
	{
		bDidExplode = true;

		OnRep_Exploded();

		//Add upward force
		MeshComp->AddForce(FVector::UpVector * UpwardForce, NAME_None, true);

		//Add Radial force
		RadialComp->FireImpulse();
	}
}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASExplosiveBarrel, bDidExplode);
}

