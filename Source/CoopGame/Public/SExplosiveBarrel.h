// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class URadialForceComponent* RadialComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	class UMaterialInterface* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* effect;

	UPROPERTY(EditAnywhere, Category = "Explosive Barrel")
	float UpwardForce;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bDidExplode;

	UFUNCTION()
	void OnRep_Exploded();

	UFUNCTION()
	void OnBarrelHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
